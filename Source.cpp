#include "Dxlib.h"

#define GAME_WIDTH 800
#define GAME_HEIGHT 600
#define GAME_COLOR 32

#define juryoku 4     
#define jump 10

#define GAME_BACKGROUND	"..\\‰æ‘œ\\ƒQ[ƒ€”wŒi.jpg"
#define GAME_CHARA		"..\\‰æ‘œ\\‚­‚ç‚°100.png"
#define GAME_LOGO		"..\\‰æ‘œ\\ƒQ[ƒ€ƒ^ƒCƒgƒ‹.png"
#define GAME_PUSH		"..\\‰æ‘œ\\pushenter.png"

#define OP_BGM			"..\\BGM\\op.mp3"
#define WAVE_BGM		"..\\BGM\\wave.mp3"
#define PLAY_BGM		"..\\BGM\\play.mp3"
#define END_BGM			"..\\BGM\\end.mp3"

#define GAME_FPS_SPEED 60

#define SET_WINDOW_ST_MODE_DEFAULT 0
#define SET_WINDOW_ST_MODE_TITLE_NONE 1
#define SET_WINDOW_ST_MODE_TITLE_FLAME_NONE 2
#define SET_WINDOW_ST_MODE_FLAME_NONE 3

enum GAME_SCENE {
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END
};

//########## \‘¢‘Ì ##########
struct CHARA {
	int Handle;
	char FilePath[128];
	int X;
	int Y;
	int Width;
	int Height;
	int MoveSpeed;
	BOOL IsView;

	BOOL IsJump;
	int jump_cnt;
	int jump_max;
};

struct GOAL {
	int Handle;
	int X;
	int Y;
	int Width;
	int Height;
	int C_Width;
	int C_Height;
};

struct KABE {
	RECT kabe1;
	RECT kabe2;
};

CHARA chara;
GOAL goal;
KABE kabe;

//fpsŠÖ˜A
int CountFps;
int StartTimeFps;
int SampleNumFps = GAME_FPS_SPEED;
float CalcFps;
int GameSceneNow = (int)GAME_SCENE_TITLE;
char AllKeyState[256];

BOOL MY_GAZOU_LOAD(CHARA*g, int x, int y, const char*path);

VOID MY_ALL_KEYDOWN_UPDATE(VOID);

VOID MY_FPS_UPDATE(VOID);
VOID MY_FPS_WAIT(VOID);
VOID MY_FPS_DRAW(VOID);

VOID MY_DRAW_PLAY_INFO(VOID);

VOID MY_GAME_TITLE(VOID);
VOID MY_GAME_END(VOID);

VOID MY_DRAW_PLAY_INFO(VOID);//ƒvƒŒƒC‰æ–Ê

int Handle_back;
int Handle_title;
int Handle_pushenter;

int BGM_nami_handle;	//BGM‚Ìƒnƒ“ƒhƒ‹
int BGM_op_handle;
int BGM_play_handle;
int BGM_end_handle;

int goalX, goalY, goalW, goalH;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	ChangeWindowMode(TRUE);
	SetWindowStyleMode(SET_WINDOW_ST_MODE_DEFAULT);

	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	if (MY_GAZOU_LOAD(&chara, 100, 400, GAME_CHARA) == FALSE) { return -1; }
	Handle_back = LoadGraph(GAME_BACKGROUND);//”wŒi
	Handle_title = LoadGraph(GAME_LOGO);//ƒ^ƒCƒgƒ‹
	Handle_pushenter = LoadGraph(GAME_PUSH);//‚¦‚ñ‚½[‚«[‚¨‚µ‚Ä

	BGM_nami_handle = LoadSoundMem(WAVE_BGM);
	BGM_op_handle = LoadSoundMem(OP_BGM);
	BGM_play_handle = LoadSoundMem(PLAY_BGM);
	BGM_end_handle = LoadSoundMem(END_BGM);

	if (BGM_nami_handle == -1) { return -1; }
	if (BGM_op_handle == -1) { return -1; }
	if (BGM_play_handle == -1) { return -1; }
	if (BGM_end_handle == -1) { return -1; }


	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }

		MY_ALL_KEYDOWN_UPDATE();
		MY_FPS_UPDATE();

		switch (GameSceneNow)
		{
		case(int)GAME_SCENE_TITLE:
			MY_GAME_TITLE();

			break;
		case(int)GAME_SCENE_PLAY:



			chara.Y += juryoku;

			if (AllKeyState[KEY_INPUT_SPACE] != 0)
			{

				if (chara.IsJump == FALSE)
				{
					chara.IsJump = TRUE;
				}
			}

			if (chara.IsJump == TRUE)
			{
				chara.jump_cnt++;

				if (chara.Y - chara.MoveSpeed > 0)
				{
					chara.Y -= chara.MoveSpeed;
				}
			}

			if (chara.jump_cnt == chara.jump_max)
			{
				chara.IsJump = FALSE;
				chara.jump_cnt = 0;
			}






			if (chara.Y + chara.Height + chara.MoveSpeed > GAME_HEIGHT)
			{
				chara.Y -= juryoku;
			}






			MY_DRAW_PLAY_INFO();

			DrawGraph(0, 0, Handle_back, TRUE);
			DrawGraph(chara.X, chara.Y, chara.Handle, TRUE);
			DrawFormatString(0, 40, GetColor(0, 0, 0), "ƒNƒ‰ƒQ‚ÌˆÊ’uF(%3d,%3d)", chara.X, chara.Y);
			DrawFormatString(0, 20, GetColor(0, 0, 0), "ƒWƒƒƒ“ƒvŽžŠÔF%3d", chara.jump_cnt);
		}
		MY_FPS_DRAW();

		ScreenFlip();

		MY_FPS_WAIT();
	}

	DeleteSoundMem(BGM_nami_handle);

	WaitKey();
	DxLib_End();

	return 0;
}

BOOL MY_GAZOU_LOAD(CHARA*g, int x, int y, const char*path)
{
	wsprintf(g->FilePath, path);
	g->Handle = LoadGraph(g->FilePath);

	if (g->Handle == -1) { return FALSE; }

	GetGraphSize(g->Handle, &g->Width, &g->Height);
	g->X = x;
	g->Y = y;

	g->MoveSpeed = jump;
	g->IsView = TRUE;

	return TRUE;
}

VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0)
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == GAME_FPS_SPEED)
	{
		int now = GetNowCount();
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)GAME_FPS_SPEED);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

VOID MY_FPS_DRAW(VOID)
{
	DrawFormatString(0, 0, GetColor(0, 0, 0), "FPS:%.1f", CalcFps);

	return;
}

VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;
	int waitTime = CountFps * 1000 / GAME_FPS_SPEED - resultTime;

	if (waitTime > 0)
	{
		Sleep(waitTime);
	}
	return;
}

VOID MY_GAME_TITLE(VOID)
{

	//”g‚Ì‰¹‚ª—¬‚ê‚Ä‚¢‚È‚©‚Á‚½‚ç—¬‚·
	if (CheckSoundMem(BGM_nami_handle) == 0)
	{
		PlaySoundMem(BGM_nami_handle, DX_PLAYTYPE_LOOP);
	}
	if (CheckSoundMem(BGM_op_handle) == 0)
	{
		PlaySoundMem(BGM_op_handle, DX_PLAYTYPE_LOOP);
	}

	if (AllKeyState[KEY_INPUT_RETURN] != 0)
	{
		//”g‚Ì‰¹‚ª—¬‚ê‚Ä‚¢‚ê‚ÎŽ~‚ß‚é
		if (CheckSoundMem(BGM_nami_handle) == 1)
		{
			StopSoundMem(BGM_nami_handle);
		}
		if (CheckSoundMem(BGM_op_handle) == 1)
		{
			StopSoundMem(BGM_op_handle);
		}


		chara.X = 100;
		chara.Y = 200;

		chara.IsJump = FALSE;
		chara.jump_cnt = 0;
		chara.jump_max = 25;	//60fps=1•b

		kabe.kabe1.left = 700;
		kabe.kabe1.top = 0;
		kabe.kabe1.right = 800;
		kabe.kabe1.bottom = 200;

		kabe.kabe2.left = 700;
		kabe.kabe2.top = 400;
		kabe.kabe2.right = 800;
		kabe.kabe2.bottom = 600;

		GameSceneNow = (int)GAME_SCENE_PLAY;
	}

	DrawGraph(0, 0, Handle_back, TRUE);
	DrawGraph(0, 0, Handle_title, TRUE);
	DrawGraph(0, 220, Handle_pushenter, TRUE);

}

VOID MY_GAME_END(VOID)
{
	char EndWin[64] = "GAME CLEAR";
	char EndLose[64] = "GAME OVER";
	SetFontSize(100);
	DrawString(150, 200, &EndWin[0], GetColor(255, 255, 255));

	/*if (endbgm_flag == true)
	{
		PlayMusic(END_BGM, DX_PLAYTYPE_BACK);
		endbgm_flag = false;
	}*/

	if (CheckSoundMem(BGM_end_handle) == 0)
	{
		PlaySoundMem(BGM_end_handle, DX_PLAYTYPE_LOOP);
	}

	if (AllKeyState[KEY_INPUT_RETURN != 0])
	{
		if (CheckSoundMem(BGM_end_handle) == 1)
		{
			StopSoundMem(BGM_end_handle);
		}
		GameSceneNow = (int)GAME_SCENE_TITLE;
	}

	SetFontSize(24);
}

VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];
	GetHitKeyStateAll(TempKey);
	for (int i = 0; i < 256; i++)
	{

		if (TempKey[i] != 0)
		{
			AllKeyState[i]++;
		}
		else
		{
			AllKeyState[i] = 0;
		}
	}
	return;
}

VOID MY_DRAW_PLAY_INFO(VOID)
{

	/*if (playbgm_flag == true)
	{
		PlayMusic(PLAY_BGM, DX_PLAYTYPE_LOOP);
		playbgm_flag = false;
	}*/

	if (CheckSoundMem(BGM_play_handle) == 0)
	{
		PlaySoundMem(BGM_play_handle, DX_PLAYTYPE_LOOP);
	}

	if (AllKeyState[KEY_INPUT_RETURN] != 0)
	{
		//”g‚Ì‰¹‚ª—¬‚ê‚Ä‚¢‚ê‚ÎŽ~‚ß‚é
		if (CheckSoundMem(BGM_play_handle) == 1)
		{
			StopSoundMem(BGM_play_handle);
		}
	}

	//ƒS[ƒ‹”ÍˆÍ
	DrawBox(goal.X, goal.Y, goal.X + goal.Width, goal.Y + goal.Height, GetColor(255, 0, 0), true);

	//“–‚½‚è”»’è
	if (chara.X < goal.X + goal.Width &&
		chara.Y < goal.Y + goal.Height &&
		chara.X + chara.Width > goal.X &&
		chara.Y + chara.Height > goal.Y)
	{
		GameSceneNow = (int)GAME_SCENE_END;
	}
}