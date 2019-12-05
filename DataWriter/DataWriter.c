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
void writeEntry(FILE* file, GameDataEntry* entry);
int main()
{
	FILE* dataFile = fopen("gamedata.txt", "w");
	if (dataFile == NULL) {
		perror("데이터파일을 만들 수 없습니다");
		return 1;
	}

	const clock_t CLOCKS_PER_50MS = CLOCKS_PER_SEC / 20;
	KeyInputs keyInputs;
	GameDataEntry entry;
	int serialBefore = 1;
	resetEntry(&entry);

	PlaySoundA("music.wav", NULL, SND_ASYNC);
	clock_t start = clock();
	while (1) {
		int serial = (clock() - start) / CLOCKS_PER_50MS + 1;
		getKeyInputs(&keyInputs);
		makeEntry(&keyInputs, &entry);
		if (keyInputs.control) {
			fclose(dataFile);
			return 0;
		}
		if (serialBefore != serial) {
			entry.serial = serialBefore;
			printf("%5d    %c    %c    %c    %c\n", entry.serial, BOOL_TO_STAR(entry.left), BOOL_TO_STAR(entry.up), BOOL_TO_STAR(entry.down), BOOL_TO_STAR(entry.right));
			writeEntry(dataFile, &entry);
			resetEntry(&entry);
			serialBefore = serial;
		}
	}
	// Do something here
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
void writeEntry(FILE* file, GameDataEntry* entry) {
	fprintf(file, "%d,%d,%d,%d,%d\n", entry->serial, entry->left, entry->up, entry->down, entry->right);
}