#pragma comment(lib,"winmm") 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <time.h>
#define FALSE 0
#define TRUE 1
#define BOOL_TO_STAR(_a) ((_a) ? '*' : ' ')

typedef char bool;
typedef struct _KEY_INPUTS {
	bool left;
	bool up;
	bool down;
	bool right;
	bool control;
} KeyInputs;

typedef struct _GAME_DATA_ENTRY {
	int serial;
	bool left;
	bool up;
	bool down;
	bool right;
} GameDataEntry;

void getKeyInputs(KeyInputs *data);
void makeEntry(KeyInputs *inputs, GameDataEntry *entry);
void resetEntry(GameDataEntry *entry);
char *hit_miss(int a);

int main() {
	//system("mode con lines=24");
	FILE* dataFile = fopen("gamedata.txt", "r");
	if (dataFile == NULL) {
		perror("데이터파일을 읽을 수 없습니다");
		return 1;
	}

	int score = 0;
	const clock_t CLOCKS_PER_50MS = CLOCKS_PER_SEC / 20;
	KeyInputs keyInputs;
	GameDataEntry entry;
	GameDataEntry arr[24];
	int serialBefore = 1;
	resetEntry(&entry);

	PlaySoundA("music.wav", NULL, SND_ASYNC);
	clock_t start = clock();
	printf("잠시후 게임이 시작됩니다.\n");
	Sleep(1000);
	while (!feof(dataFile)) {
		int serial = (clock() - start) / CLOCKS_PER_50MS + 1;
		int user_serial = serial - 24;
		getKeyInputs(&keyInputs);
		makeEntry(&keyInputs, &entry);
		if (serialBefore != serial) {
			GameDataEntry tmpdata;
			fscanf(dataFile, "%d,%c,%c,%c,%c\n", &tmpdata.serial, &tmpdata.left, &tmpdata.up, &tmpdata.down, &tmpdata.right);
			tmpdata.left -= '0';
			tmpdata.up -= '0';
			tmpdata.down -= '0';
			tmpdata.right -= '0';
			int flagl = 0, flagu = 0, flagd = 0, flagr = 0;
			if (user_serial > 0) {
				if (entry.left == 1 && arr[0].left == 1) score++, flagl = 1;
				if (entry.up == 1 && arr[0].up == 1) score++, flagu = 1;
				if (entry.down == 1 && arr[0].down == 1) score++, flagd = 1;
				if (entry.right == 1 && arr[0].right == 1) score++, flagr = 1;

				if (entry.left == 1 && arr[0].left == 0) score--, flagl = 2;
				if (entry.up == 1 && arr[0].up == 0) score--, flagu = 2;
				if (entry.down == 1 && arr[0].down == 0) score--, flagd = 2;
				if (entry.right == 1 && arr[0].right == 0) score--, flagr = 2;

				if (entry.left == 0 && arr[0].left == 1) score--, flagl = 2;
				if (entry.up == 0 && arr[0].up == 1) score--, flagu = 2;
				if (entry.down == 0 && arr[0].down == 1) score--, flagd = 2;
				if (entry.right == 0 && arr[0].right == 1) score--, flagr = 2;

				for (int i = 1; i < 24; i++) arr[i - 1] = arr[i];
				arr[23] = tmpdata;
			}
			else {
				arr[serial - 1] = tmpdata;
			}
			entry.serial = serialBefore;
			printf("%05d    %c     %c     %c    %c", tmpdata.serial, BOOL_TO_STAR(tmpdata.left), BOOL_TO_STAR(tmpdata.up), BOOL_TO_STAR(tmpdata.down), BOOL_TO_STAR(tmpdata.right));
			printf("%s %s %s %s\n", hit_miss(flagl), hit_miss(flagu), hit_miss(flagd), hit_miss(flagr));
			resetEntry(&entry);
			serialBefore = serial;
		}
	}
	fclose(dataFile);
	int loopcnt = 24;
	while (loopcnt) {
		int serial = (clock() - start) / CLOCKS_PER_50MS + 1;
		int user_serial = serial - 24;
		getKeyInputs(&keyInputs);
		makeEntry(&keyInputs, &entry);
		if (serialBefore != serial) {
			loopcnt--;
			int flagl = 0, flagu = 0, flagd = 0, flagr = 0;
			if (user_serial >= 0) {
				if (entry.left == 1 && arr[0].left == 1) score++, flagl = 1;
				if (entry.up == 1 && arr[0].up == 1) score++, flagu = 1;
				if (entry.down == 1 && arr[0].down == 1) score++, flagd = 1;
				if (entry.right == 1 && arr[0].right == 1) score++, flagr = 1;

				if (entry.left == 1 && arr[0].left == 0) score--, flagl = 2;
				if (entry.up == 1 && arr[0].up == 0) score--, flagu = 2;
				if (entry.down == 1 && arr[0].down == 0) score--, flagd = 2;
				if (entry.right == 1 && arr[0].right == 0) score--, flagr = 2;

				if (entry.left == 0 && arr[0].left == 1) score--, flagl = 2;
				if (entry.up == 0 && arr[0].up == 1) score--, flagu = 2;
				if (entry.down == 0 && arr[0].down == 1) score--, flagd = 2;
				if (entry.right == 0 && arr[0].right == 1) score--, flagr = 2;

				for (int i = 1; i < 24; i++) arr[i - 1] = arr[i];
			}
			entry.serial = serialBefore;
			//printf("           %c      %c      %c     %c", BOOL_TO_STAR(entry.left), BOOL_TO_STAR(entry.up), BOOL_TO_STAR(entry.down), BOOL_TO_STAR(entry.right));
			printf("                           %s %s %s %s\n", hit_miss(flagl), hit_miss(flagu), hit_miss(flagd), hit_miss(flagr));
			resetEntry(&entry);
			serialBefore = serial;
		}
	}
	printf("게임이 끝났습니다. 잠시후 결과가 표시됩니다.\n");
	Sleep(1000);
	printf("%d", score);
	return 0;
}

void getKeyInputs(KeyInputs *data) {
	data->left = GetAsyncKeyState(VK_LEFT) & 0x8000 ? TRUE : FALSE;
	data->up = GetAsyncKeyState(VK_UP) & 0x8000 ? TRUE : FALSE;
	data->down = GetAsyncKeyState(VK_DOWN) & 0x8000 ? TRUE : FALSE;
	data->right = GetAsyncKeyState(VK_RIGHT) & 0x8000 ? TRUE : FALSE;
	data->control = GetAsyncKeyState(VK_CONTROL) & 0x8000 ? TRUE : FALSE;
}

void makeEntry(KeyInputs *inputs, GameDataEntry *entry) {
	if (inputs->down)
		entry->down = TRUE;
	if (inputs->left)
		entry->left = TRUE;
	if (inputs->right)
		entry->right = TRUE;
	if (inputs->up)
		entry->up = TRUE;
}

void resetEntry(GameDataEntry *entry) {
	entry->down = FALSE;
	entry->left = FALSE;
	entry->right = FALSE;
	entry->up = FALSE;
	entry->serial = 0;
}

char *hit_miss(int a) {
	if (a == 0) return "    ";
	else if (a == 1) return " hit";
	else if (a == 2)return "miss";
}