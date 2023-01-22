#include "buffy.h"
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define BULLET_SPEED 0.8

int main(void) {
	key_t file_key = ftok("./mem.share", 1);
	pid_t keygetter = fork();
	assert(keygetter != -1);
	kbkey *key;

	if (keygetter) { // parrent
		int mem_block = shmget(file_key, sizeof(kbkey), IPC_CREAT);
		key = shmat(mem_block, NULL, 0);
	} else { // child
		StartChTerm(NULL);
		int mem_block = shmget(file_key, sizeof(kbkey), 0);
		key = shmat(mem_block, NULL, 0);
		while (*key != KEY_f9) {
			*key = GetChId();
		}
	}
	*key = rand();

	// speed
	polar heading= MakePolar(9, 90);

	polar pl_head= MakePolar(20, 90);
	polar pl_lleg= MakePolar(20, 290);
	polar pl_rleg= MakePolar(20, 250);

	point at = MakePoint(500, 500);

	point head;
	point lleg;
	point rleg;

	//TODO: support many shots
	//line *shots[30] = {0};
	line shot = MakeLine(__BUFFY_H_POINT_ZZ, __BUFFY_H_POINT_ZZ);
	point temp;


	struct fbjar jar = InitBuffy();
	while (*key!=KEY_esc) {
		head = pMovePoint(plMakePoint(pl_head), at);
		lleg = pMovePoint(plMakePoint(pl_lleg), at);
		rleg = pMovePoint(plMakePoint(pl_rleg), at);

		DrawLine(jar, shot, 100, RGB(0,0,0));
		temp = pMakePoint(D1LineLerp(shot, 1+BULLET_SPEED));
		shot.a = pMakePoint(D1LineLerp(shot, BULLET_SPEED));
		shot.b = temp;
		if (!CheckPIJ(jar, shot.b)) {
			shot.a = __BUFFY_H_POINT_ZZ;
			shot.b = __BUFFY_H_POINT_ZZ;
		}
		SDrawLine(jar, shot, 100);
	
		pDrawLine(jar, head, lleg, 60, RGB(255, 255, 255));
		pDrawLine(jar, head, rleg, 60, RGB(255, 255, 255));
		pDrawLine(jar, rleg, lleg, 30, RGB(255, 255, 255));

		fsleep(1.0/30.0);

		pDrawLine(jar, head, lleg, 60, RGB(0, 0, 0));
		pDrawLine(jar, head, rleg, 60, RGB(0, 0, 0));
		pDrawLine(jar, rleg, lleg, 30, RGB(0, 0, 0));

		if (*key == KEY_d) {
			heading = RotatePolar(heading, 9);

			pl_head = RotatePolar(pl_head, 9);
			pl_lleg = RotatePolar(pl_lleg, 9);
			pl_rleg = RotatePolar(pl_rleg, 9);
		} else if (*key == KEY_a) {
			heading = RotatePolar(heading, -9);

			pl_head = RotatePolar(pl_head, -9);
			pl_lleg = RotatePolar(pl_lleg, -9);
			pl_rleg = RotatePolar(pl_rleg, -9);
		} else if (*key == KEY_w) {
			at = pMovePoint(plMakePoint(heading), at);
		} else if (*key == KEY_s) {
			at = pSubp(at, plMakePoint(heading));
		} else if (*key == KEY_space) {
			shot = MakeLine(at, head);
		}
		if (*key) {
			*key=0;
		}
	}

	shmdt(key);
	StopBuffy(jar);
	return 0;
}

