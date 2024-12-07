#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

void uid() {
	uid_t user_id = getuid();
	gid_t group_id = getgid();
	struct passwd *pw = getpwuid(user_id);
	struct group *gr = getgrgid(group_id);
	printf("uid=%d [%s]\ngid=%d [%s]\n", user_id, pw->pw_name, group_id, gr->gr_name);
}

void euid() {
	uid_t effective_user_id = geteuid();
	struct passwd *pw = getpwuid(effective_user_id);
	printf("euid=%d [%s]\n", effective_user_id, pw->pw_name);
}

void groups() {
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
	uid();
	euid();
	groups();
	return 0;
}
