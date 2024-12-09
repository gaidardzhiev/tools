#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

void u() {
	uid_t ui = getuid();
	gid_t gi = getgid();
	struct passwd *p = getpwuid(ui);
	struct group *gr = getgrgid(gi);
	printf("uid=%d [%s]\ngid=%d [%s]\n", ui, p->pw_name, gi, gr->gr_name);
}

void e() {
	uid_t effective_user_id = geteuid();
	struct passwd *p = getpwuid(effective_user_id);
	printf("euid=%d [%s]\n", effective_user_id, p->pw_name);
}

void g() {
	gid_t groups[32];
	int ngroups = 32;
	getgrouplist(getpwuid(getuid())->pw_name, getgid(), groups, &ngroups);
	printf("groups=");
	for (int i = 0; i < ngroups; i++) {
		struct group *gr = getgrgid(groups[i]);
		if (gr != NULL) {
			printf("%d [%s]", groups[i], gr->gr_name);
			if (i < ngroups - 1) {
				printf("\n");
			}
		}
	}
	printf("\n");
}

int main() {
	u();
	e();
	g();
	return 0;
}
