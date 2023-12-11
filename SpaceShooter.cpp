//Kelompok 10
//Kelas B
//Anggota:	1. Muhammad Nur Faadil		   (2317051112)
//			2. Allisya Syakdari		      (2317051017)
//			3. Kgs. Muhammad Fathurrahman    (2317051086)
#include <iostream>
#include <conio.h>
#include <direct.h> 
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define Enemy_DIF 45

using namespace std; 
 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char plane[2][5] = { '|',' ',' ',' ','|',
					'^','<','0','>','^',}; 
char enemy[4][6] = {' ', '=', '=', '=', '=', ' ',
                   '/', ' ', ' ', ' ', ' ', '\\',
                   '|', ' ', ' ', ' ', ' ', '|',
                   '\\', 'v', 'v', 'v', 'v', '/'};

int planePos = WIN_WIDTH/2;
int score = 0;
int bullets[20][4];
int bulletsLife[20];
int bIndex = 0;
bool space_pressed = false;
bool up_pressed = false;

//penentuan posisi
void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

//Gambar map dan bingkai
void drawBorder(){ 
	for (int i = 0; i < SCREEN_HEIGHT+1; i++) {
        for (int j = 0; j < 17; j++) {
            gotoxy(0 + j, i);
            cout << "|";
            gotoxy(WIN_WIDTH - j, i);
            cout << "|";
        }
    }
	
	for(int i=0; i<SCREEN_WIDTH+1; i++){ 
		gotoxy(i,SCREEN_HEIGHT+2); cout<<"=";
	}
	
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(0,i); cout<<"|";
		gotoxy(SCREEN_WIDTH,i); cout<<"|";
	}
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(WIN_WIDTH,i); cout<<"|";
	}
}

//ngilangin kursor di terminal
void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}

//random posisi looping enemy
void genEnemy(int ind){
	enemyX[ind] = 17 + rand()%(33);  
}

//gambar enemy kedalam array
void drawEnemy(int ind){
	if (enemyFlag[ind]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 6; j++) {
                gotoxy(enemyX[ind] + j, enemyY[ind] + i);
                cout << enemy[i][j];
            }
        }
    }
}

//hapus enemy kalo kena peluru
void eraseEnemy(int ind){
	if (enemyFlag[ind]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 6; j++) {
                gotoxy(enemyX[ind] + j, enemyY[ind] + i);
                cout << ' ';
            }
        }
    }
}

//reset enemy kalo mentok ke bawah
void resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 4;
	genEnemy(ind);
}
//posisi peluru
void genBullet(){
	bullets[bIndex][0] = 22;
	bullets[bIndex][1] = planePos;
	bullets[bIndex][2] = 22; 
	bullets[bIndex][3] = planePos+4; 
	bIndex++;
	if( bIndex == 20)
		bIndex = 0;
}

//gerak peluru 
void moveBullet(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] > 2 )
			bullets[i][0]--;
		else
			bullets[i][0] = 0;
		
		if( bullets[i][2] > 2 )
			bullets[i][2]--;
		else
			bullets[i][2] = 0;
	} 
}
//gambar peluru ke atas
void drawBullets(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] > 1){
			gotoxy(bullets[i][1],bullets[i][0]); cout<<"^"; 
			gotoxy(bullets[i][3],bullets[i][2]); cout<<"^";
		}
	}
}

//apus peluru kalo mentok ke atas
void eraseBullets(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] >= 1 ){
			gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
			gotoxy(bullets[i][3],bullets[i][2]); cout<<" ";
		}
	}
}
//apus peluru kalo kena enemy
void eraseBullet(int i){ 
	gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
	gotoxy(bullets[i][3],bullets[i][2]); cout<<" "; 
}

//gambar pesawat
void drawPlane(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			gotoxy(j+planePos, i+22); cout<<plane[i][j];
		}
	}
}

//apus pesawat
void erasePlane(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			gotoxy(j+planePos, i+22); cout<<" ";
		}
	}
}
 
 //keadaan kalah
int collision(){
	if( enemyY[0]+4 >= 23 ){
		if( enemyX[0] + 4 - planePos >= 0 && enemyX[0] + 4 - planePos < 8  ){
			
			return 1;
		}
	}
	return 0;
}

//keadaan peluru kena musuh
int bulletHit(){
	for(int i=0; i<20; i++){
		for(int j=0; j<4; j+=2){
			if( bullets[i][j] != 0 ){
				if( bullets[i][j] >= enemyY[0] && bullets[i][j] <= enemyY[0]+4 ){
					if( bullets[i][j+1] >= enemyX[0] && bullets[i][j+1] <= enemyX[0]+4 ){
						eraseBullet(i);
						bullets[i][j] = 0;
						resetEnemy(0);
						return 1;
					}
				}
				if( bullets[i][j] >= enemyY[1] && bullets[i][j] <= enemyY[1]+4 ){
					if( bullets[i][j+1] >= enemyX[1] && bullets[i][j+1] <= enemyX[1]+4 ){
						eraseBullet(i);
						resetEnemy(1); 
						bullets[i][j] = 0;
						return 1;
					}
				}
			}
		}
	}
	
}


//tampilan ketika kalah
void gameover(){
	system("cls");
	cout<<endl;
	cout<<"\t\t--------------------------"<<endl;
	cout<<"\t\t-------- Game Over -------"<<endl;
	cout<<"\t\t--------------------------"<<endl<<endl;
	cout<<"\t\tPencet apapun untuk kembali.";
	getch();
}

//tampilan skor
void updateScore(){
	gotoxy(WIN_WIDTH + 7,5);cout<<"Skor: "<<score<<endl;
}

//level 1
void play1(){
    
    
    planePos = -1 + WIN_WIDTH/2;
    score = 0;
    enemyFlag[0] = 1;  
    enemyFlag[1] = 1;
    enemyY[0] = enemyY[1] = 4;
    
    for(int i=0; i<20; i++){
    	
        bullets[i][0] = bullets[i][1] = 0; 
    }
    
    system("cls"); 
    drawBorder();
    updateScore();
    genEnemy(0);

  
  	gotoxy(WIN_WIDTH + 5, 2);cout<<"Spaceshooter";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	gotoxy(WIN_WIDTH + 7, 12);cout<<"Kontrol ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<" A Key - Kiri";
	gotoxy(WIN_WIDTH + 2, 15);cout<<" D Key - Kanan";
	gotoxy(WIN_WIDTH + 2, 16);cout<<" Spacebar = Tembak";
	


	
		while(1){ 
  		if(kbhit()){
  		
  		char ch = getch();
  		if( ch=='a' || ch=='A' ){
  			if( planePos > 18 )
  			    planePos -= 4;
	    }
	    if(ch =='d' || ch=='D' ){
	    	if( planePos < 50 )
	    	    planePos += 4;
		}
		if(ch ==' '|| ch==32){
			genBullet();
		}
        if(ch==27){
    	   break;
	    }
    }
        		
        drawPlane();
        drawEnemy(0);

        drawBullets();
        if( collision() == 1 ){
            gameover();
            Sleep(1000);
            return;
        }

        if( bulletHit() == 1 ){
            score++;
            updateScore(); 
        } 
        Sleep(500);
        erasePlane();
        eraseEnemy(0);
        eraseEnemy(1);
        eraseBullets();
        moveBullet();   
        
        if( enemyFlag[0] == 1 )
            enemyY[0] += 1;
        
        if( enemyFlag[1] == 1 )
            enemyY[1] += 1;
         
        if( enemyY[0] > SCREEN_HEIGHT-5 ){ 
            resetEnemy(0); 
        }
        if( enemyY[1] > SCREEN_HEIGHT-5 ){ 
            resetEnemy(1); 
        }
    }
    
}

//level 2
void play2(){
    
    
    planePos = -1 + WIN_WIDTH/2;
    score = 0;
    enemyFlag[0] = 1;  
    enemyFlag[1] = 1;
    enemyY[0] = enemyY[1] = 4;
    
    for(int i=0; i<20; i++){
    	
        bullets[i][0] = bullets[i][1] = 0; 
    }
    
    system("cls"); 
    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);
  
  	gotoxy(WIN_WIDTH + 5, 2);cout<<"Spaceshooter";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	gotoxy(WIN_WIDTH + 7, 12);cout<<"Kontrol ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<" A Key - Kiri";
	gotoxy(WIN_WIDTH + 2, 15);cout<<" D Key - Kanan";
	gotoxy(WIN_WIDTH + 2, 16);cout<<" Spacebar = Tembak";
	


	
		while(1){ 
  		if(kbhit()){
  		
  		char ch = getch();
  		if( ch=='a' || ch=='A' ){
  			if( planePos > 18 )
  			    planePos -= 4;
	    }
	    if(ch =='d' || ch=='D' ){
	    	if( planePos < 50 )
	    	    planePos += 4;
		}
		if(ch ==' '|| ch==32){
			genBullet();
		}
        if(ch==27){
    	   break;
	    }
    }
        
        
        
        drawPlane();
        drawEnemy(0);
        drawEnemy(1);
        drawBullets();
        if( collision() == 1 ){
            gameover();
            Sleep(1000);
            return;
        }

        if( bulletHit() == 1 ){
            score++;
            updateScore(); 
        } 
        Sleep(500);
        erasePlane();
        eraseEnemy(0);
        eraseEnemy(1);
        eraseBullets();
        moveBullet();   
        
        if( enemyFlag[0] == 1 )
            enemyY[0] += 1;
        
        if( enemyFlag[1] == 1 )
            enemyY[1] += 1;
         
        if( enemyY[0] > SCREEN_HEIGHT-4 ){ 
            resetEnemy(0); 
        }
        if( enemyY[1] > SCREEN_HEIGHT-4 ){ 
            resetEnemy(1); 
        }
    }
    
}

//level 3
void play3(){
    
    
    planePos = -1 + WIN_WIDTH/2;
    score = 0;
    enemyFlag[0] = 1;  
    enemyFlag[1] = 1;
    enemyY[0] = enemyY[1] = 4;
    
    for(int i=0; i<20; i++){
    	
        bullets[i][0] = bullets[i][1] = 0; 
    }
    
    system("cls"); 
    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);
  
  	gotoxy(WIN_WIDTH + 5, 2);cout<<"Spaceshooter";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	gotoxy(WIN_WIDTH + 7, 12);cout<<"Kontrol ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<" A Key - Kiri";
	gotoxy(WIN_WIDTH + 2, 15);cout<<" D Key - Kanan";
	gotoxy(WIN_WIDTH + 2, 16);cout<<" Spacebar = Tembak";
	

		while(1){ 
  		if(kbhit()){
  		
  		char ch = getch();
  		if( ch=='a' || ch=='A' ){
  			if( planePos > 18 )
  			    planePos -= 4;
	    }
	    if(ch =='d' || ch=='D' ){
	    	if( planePos < 50 )
	    	    planePos += 4;
		}
		if(ch ==' '|| ch==32){
			genBullet();
		}
        if(ch==27){
    	   break;
	    }
    }
        
        
        
        drawPlane();
        
        drawEnemy(0);
        drawEnemy(1);
        drawBullets();
        if( collision() == 1 ){
            gameover();
            Sleep(1000);
            return;
        }

        if( bulletHit() == 1 ){
            score++;
            updateScore(); 
        } 
        Sleep(50);
        erasePlane();
        eraseEnemy(0);
        eraseEnemy(1);
        eraseBullets();
        moveBullet();   
        
        if( enemyFlag[0] == 1 )
            enemyY[0] += 1;
        
        if( enemyFlag[1] == 1 )
            enemyY[1] += 1;
         
        if( enemyY[0] > SCREEN_HEIGHT-4 ){ 
            resetEnemy(0); 
        }
        if( enemyY[1] > SCREEN_HEIGHT-4 ){ 
            resetEnemy(1); 
        }
    }
    
}

//level 4
void play4(){
    
    planePos = -1 + WIN_WIDTH/2;
    score = 0;
    enemyFlag[0] = 1;  
    enemyFlag[1] = 1;
    enemyY[0] = enemyY[1] = 4;
    
    for(int i=0; i<20; i++){
    	
        bullets[i][0] = bullets[i][1] = 0; 
    }
    
    system("cls"); 
    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);
  
  	gotoxy(WIN_WIDTH + 5, 2);cout<<"Spaceshooter";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	gotoxy(WIN_WIDTH + 7, 12);cout<<"Kontrol ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<" A Key - Kiri";
	gotoxy(WIN_WIDTH + 2, 15);cout<<" D Key - Kanan";
	gotoxy(WIN_WIDTH + 2, 16);cout<<" Spacebar = Tembak";
	


	
		while(1){ 
  		if(kbhit()){
  		
  		char ch = getch();
  		if( ch=='a' || ch=='A' ){
  			if( planePos > 18 )
  			    planePos -= 4;
	    }
	    if(ch =='d' || ch=='D' ){
	    	if( planePos < 50 )
	    	    planePos += 4;
		}
		if(ch ==' '|| ch==32){
			genBullet();
		}
        if(ch==27){
    	   break;
	    }
    }
        
        
        
        drawPlane();
        
        drawEnemy(0);
        drawEnemy(1);
        drawBullets();
        if( collision() == 1 ){
            gameover();
            Sleep(1000);
            return;
        }

        if( bulletHit() == 1 ){
            score++;
            updateScore(); 
        } 
        Sleep(20);
        erasePlane();
        eraseEnemy(0);
        eraseEnemy(1);
        eraseBullets();
        moveBullet();   
        
        if( enemyFlag[0] == 1 )
            enemyY[0] += 1;
        
        if( enemyFlag[1] == 1 )
            enemyY[1] += 1;
         
        if( enemyY[0] > SCREEN_HEIGHT-4 ){ 
            resetEnemy(0); 
        }
        if( enemyY[1] > SCREEN_HEIGHT-4 ){ 
            resetEnemy(1); 
        }
    }
    
}

//tampilan pilih level
void lvl(){
	system("cls");
	drawBorder();
    updateScore();

	gotoxy(24, 5);cout<<"Pilih Difficulty";
	gotoxy(24, 6);cout<<"1.Ez";
    gotoxy(24, 7);cout<<"2.mayan";
    gotoxy(24, 8);cout<<"3.susah";
	gotoxy(24, 8);cout<<"4.bruh";
	char lv=getch();

	gotoxy(24, 5); cout<<"               ";
	gotoxy(24, 6);cout<<"    ";
    gotoxy(24, 7);cout<<"       ";
    gotoxy(24, 8);cout<<"       ";
	gotoxy(24, 8);cout<<"      "; 
	if (lv==1){
		play1();
	}
	if (lv==2){
		play2();
	}
	if (lv==3){
		play3();
	}
	if (lv==4){
		play4();
	}
}


//int main buat eksekusi semua void
int main()
{
	setcursor(0,0); 
	srand( (unsigned)time(NULL)); 
	 
	do{
		system("cls");
		gotoxy(10,5); cout<<" -------------------------- "; 
		gotoxy(10,6); cout<<" |     Space Shooter      | "; 
		gotoxy(10,7); cout<<" --------------------------";
		gotoxy(10,9); cout<<"Pilih: ";
		gotoxy(10,10); cout<<"1. Mulai";
		gotoxy(10,11); cout<<"2. Keluar";
		char op = getch();
		
		if( op=='1'){
			system("cls");
	drawBorder();
    updateScore();

	gotoxy(24, 5);cout<<"Pilih Difficulty";
	gotoxy(24, 6);cout<<"1.Ez";
    gotoxy(24, 7);cout<<"2.mayan";
    gotoxy(24, 8);cout<<"3.susah";
	gotoxy(24, 9);cout<<"4.bruh";

	
	switch(getch()){
		case '1' : play1();
		break;

		case '2' : play2();
		break;
		case '3' : play3();
		break;
		case '4' : play4();
		break;
	}

	}else if( op=='2') exit(0);
		
	}while(1);
	
	return 0;
}
