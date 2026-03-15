/*****************************************************************************/

#include <sys/types.h>
#include <unistd.h>
#include <stropts.h>
#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int __streams_getpmsg_r(int fd, struct strbuf *ctlptr, struct strbuf *datptr, int *bandp,
			       int *flagsp);
extern int __streams_putpmsg_r(int fd, const struct strbuf *ctlptr, const struct strbuf *datptr,
			       int band, int flags);

static int mock_errno;
static int mock_fd;
static unsigned long mock_request;
static int mock_ioctl_calls;
static int mock_read_calls;
static int mock_write_calls;

ssize_t
read(int fd, void *buf, size_t count)
{
	(void) fd;
	(void) buf;
	(void) count;
	mock_read_calls++;
	errno = ENOSYS;
	return (-1);
}

ssize_t
write(int fd, const void *buf, size_t count)
{
	(void) fd;
	(void) buf;
	(void) count;
	mock_write_calls++;
	errno = ENOSYS;
	return (-1);
}

int
ioctl(int fd, unsigned long request, ...)
{
	va_list ap;
	void *arg;

	mock_ioctl_calls++;
	mock_fd = fd;
	mock_request = request;

	va_start(ap, request);
	arg = va_arg(ap, void *);
	va_end(ap);

	switch (request) {
	case I_GETPMSG:{
			struct strpmsg *msg = arg;

			if (msg == NULL || msg->ctlbuf.maxlen != 8 || msg->ctlbuf.len != -1
			    || msg->databuf.maxlen != 16 || msg->databuf.len != -1
			    || msg->band != 17 || msg->flags != RS_HIPRI) {
				errno = EINVAL;
				return (-1);
			}
			msg->ctlbuf.len = 3;
			msg->databuf.len = 5;
			msg->band = 4;
			msg->flags = 0;
			return (0);
		}
	case I_PUTPMSG:{
			const struct strpmsg *msg = arg;

			if (msg == NULL || msg->ctlbuf.len != 4 || msg->databuf.len != 6
			    || msg->band != 2 || msg->flags != 0
			    || memcmp(msg->ctlbuf.buf, "ctrl", 4) != 0
			    || memcmp(msg->databuf.buf, "payload", 6) != 0) {
				errno = EINVAL;
				return (-1);
			}
			return (0);
		}
	case I_ISASTREAM:
		errno = mock_errno;
		return (-1);
	default:
		errno = ENOTTY;
		return (-1);
	}
}

#include "../lib/dlpi.c"

static void
reset_mock_state(void)
{
	mock_errno = ENOSYS;
	mock_fd = -1;
	mock_request = 0;
	mock_ioctl_calls = 0;
	mock_read_calls = 0;
	mock_write_calls = 0;
}

static int
test_dlpi_linkname(void)
{
	struct __dlpi_user dh;
	char *linkname;
	const char *ret;
	int i;

	memset(&dh, 0, sizeof(dh));
	if (pthread_rwlock_init(&dh.du_lock, NULL) != 0) {
		perror("pthread_rwlock_init");
		return (1);
	}
	linkname = malloc(BUFSIZ + 1);
	if (linkname == NULL) {
		perror("malloc");
		pthread_rwlock_destroy(&dh.du_lock);
		return (1);
	}
	memset(linkname, 'A', BUFSIZ);
	linkname[BUFSIZ] = '\0';

	dh.du_fd = 7;
	dh.du_linkname = linkname;
	__dlpi_dhs[dh.du_fd] = &dh;

	ret = __dlpi_linkname_r(&dh);

	__dlpi_dhs[dh.du_fd] = NULL;
	pthread_rwlock_destroy(&dh.du_lock);
	free(linkname);

	if (ret == NULL) {
		fprintf(stderr, "dlpi_linkname_r returned NULL\n");
		return (1);
	}
	if (ret[BUFSIZ - 1] != '\0') {
		fprintf(stderr, "dlpi_linkname_r did not terminate output\n");
		return (1);
	}
	if (strlen(ret) != BUFSIZ - 1) {
		fprintf(stderr, "dlpi_linkname_r produced wrong output length: %zu\n", strlen(ret));
		return (1);
	}
	for (i = 0; i < BUFSIZ - 1; i++) {
		if (ret[i] != 'A') {
			fprintf(stderr, "dlpi_linkname_r corrupted output at %d\n", i);
			return (1);
		}
	}
	return (0);
}

static int
test_getpmsg_ioctl(void)
{
	char ctrlbuf[8];
	char databuf[16];
	struct strbuf ctrl = { 8, -1, ctrlbuf };
	struct strbuf data = { 16, -1, databuf };
	int band = 17;
	int flags = RS_HIPRI;

	reset_mock_state();
	if (__streams_getpmsg_r(123, &ctrl, &data, &band, &flags) != 0) {
		perror("__streams_getpmsg_r");
		return (1);
	}
	if (mock_ioctl_calls != 1 || mock_request != I_GETPMSG || mock_fd != 123) {
		fprintf(stderr, "getpmsg did not issue the expected ioctl\n");
		return (1);
	}
	if (mock_read_calls != 0 || mock_write_calls != 0) {
		fprintf(stderr, "getpmsg unexpectedly used read/write fallback\n");
		return (1);
	}
	if (ctrl.len != 3 || data.len != 5 || band != 4 || flags != 0) {
		fprintf(stderr, "getpmsg did not copy ioctl results back to callers\n");
		return (1);
	}
	return (0);
}

static int
test_putpmsg_ioctl(void)
{
	char ctrlbuf[] = "ctrl";
	char databuf[] = "payload";
	struct strbuf ctrl = { sizeof(ctrlbuf) - 1, 4, ctrlbuf };
	struct strbuf data = { sizeof(databuf) - 1, 6, databuf };

	reset_mock_state();
	if (__streams_putpmsg_r(321, &ctrl, &data, 2, 0) != 0) {
		perror("__streams_putpmsg_r");
		return (1);
	}
	if (mock_ioctl_calls != 1 || mock_request != I_PUTPMSG || mock_fd != 321) {
		fprintf(stderr, "putpmsg did not issue the expected ioctl\n");
		return (1);
	}
	if (mock_read_calls != 0 || mock_write_calls != 0) {
		fprintf(stderr, "putpmsg unexpectedly used read/write fallback\n");
		return (1);
	}
	return (0);
}

static void
show_usage(const char *argv0)
{
	fprintf(stdout, "Usage: %s [--help] [--version] [--copying] {dlpi-linkname|getpmsg-ioctl|putpmsg-ioctl}\n",
		argv0);
}

static void
show_version(void)
{
	fprintf(stdout, "%s (%s) %s\n", "test-lib-regress", PACKAGE_ENVR, PACKAGE_DATE);
}

static void
show_copying(void)
{
	fputs("Copyright (c) 2008-2015 Monavacon Limited\n"
	      "Copyright (c) 2001-2008 OpenSS7 Corporation\n"
	      "Copyright (c) 1997-2001 Brian F. G. Bidulock\n"
	      "\n"
	      "This test program may be distributed under the GNU Affero General Public\n"
	      "License Version 3 only for testing OpenSS7 modules, drivers, and libraries.\n",
	      stdout);
}

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		show_usage(argv[0]);
		return (2);
	}
	if (strcmp(argv[1], "--help") == 0) {
		show_usage(argv[0]);
		return (0);
	}
	if (strcmp(argv[1], "--version") == 0) {
		show_version();
		return (0);
	}
	if (strcmp(argv[1], "--copying") == 0) {
		show_copying();
		return (0);
	}
	if (strcmp(argv[1], "dlpi-linkname") == 0)
		return test_dlpi_linkname();
	if (strcmp(argv[1], "getpmsg-ioctl") == 0)
		return test_getpmsg_ioctl();
	if (strcmp(argv[1], "putpmsg-ioctl") == 0)
		return test_putpmsg_ioctl();

	fprintf(stderr, "unknown test case: %s\n", argv[1]);
	return (2);
}
