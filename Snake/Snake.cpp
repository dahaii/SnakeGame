#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <mmsystem.h>
#include <string.h>

#pragma comment(lib, "winmm.lib")

#define GENISLIK 80
#define YUKSEKLIK 23
#define MAX_NAME_LENGTH 50

int alan[GENISLIK][YUKSEKLIK] = {0};
int snakeSutun[500];
int snakeSatir[500];
int map[GENISLIK][YUKSEKLIK] = {0};


void gotoxy(short x, short y) {
    HANDLE hConsoleOutput;
    COORD Cursor_Pos = {x - 1, y - 1};

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_Pos);
}

void menu() {
    system("cls");

    printf("------------------------------\n");
    printf("        YILAN OYUNU\n");
    printf("------------------------------\n");
    printf("1. Oyuna Basla\n");
    printf("2. Skor Tablosu\n");
    printf("3. Cikis\n");
    printf("Seciminizi yapin: ");

    // Ekranı başlatma
    gotoxy(83, 1);
    printf("Akdeniz Universitesi");
    gotoxy(83, 2);
    printf("Muhendislik Fakultesi");
    gotoxy(83, 3);
    printf("Elektrik Elektronik Muhendisligi");
    gotoxy(83, 4);
    printf("Dr. Ogr. Uyesi Yalcin ALBAYRAK");
    gotoxy(83, 13);
    printf("Bilgisayar Programlama II");
    gotoxy(83, 14);
    printf("Odev Grubu Final Odevi");
    gotoxy(83, 26);
    printf("Emrullah Enes VAROL");
    gotoxy(83, 27);
    printf("20210805065");
    for (int i = 1; i <= 27; i++) {
        gotoxy(81, i);
        printf("|");
    }
    gotoxy(21, 7);
    printf(" ");
}

void sinirlar(int x1, int y1, int x2, int y2, int tur) {
    int i;
    for (i = x1; i <= x2; i++) {
        alan[i][y1] = tur;
        alan[i][y2] = tur;
    }
    for (i = y1; i <= y2; i++) {
        alan[x1][i] = tur;
        alan[x2][i] = tur;
    }
}

void ekrani_bas() {
    int x, y;
    for (x = 0; x < GENISLIK; x++) {
        for (y = 0; y < YUKSEKLIK; y++) {
            gotoxy(x + 1, y + 1);
            if (alan[x][y] == 1) {
                printf("%c", 219);
            } else if (alan[x][y] == 2) {
                printf("%c", 176);
            } else if (alan[x][y] == 3) {
                printf("%c", 88);
            }
        }
    }
}

void fishing(int *n, int *s) {
    srand(time(NULL));
    *n = 4 + rand() % (GENISLIK - 8);
    *s = 4 + rand() % (YUKSEKLIK - 8);
}

void yilanHiziniAyarla(int seviye) {
    int hiz = 0; // Yilanin hareket hizi

    switch (seviye) {
        case 1: // Zorluk seviyesi 1 (Basit)
            hiz = 100; // 100 ms bekleme suresi (en yavas hiz)
            break;
        case 2: // Zorluk seviyesi 2 (Normal)
            hiz = 50; // 50 ms bekleme suresi (orta hiz)
            break;
        case 3: // Zorluk seviyesi 3 (Zor)
            hiz = 25; // 25 ms bekleme suresi (en hizli hiz)
            break;
        default: // Gecersiz zorluk seviyesi durumunda varsayilan olarak normal hizi kullan
            hiz = 50; // 50 ms bekleme süresi
            break;
    }

    Sleep(hiz);
}

void saveScore(const char* playerName, int Puan) {
    FILE* file = fopen("skorlar.txt", "a");  // "skorlar.txt" dosyasını ekleme modunda aç

    if (file == NULL) {
        printf("Dosya acilamadi.\n");
        return;
    }

    fprintf(file, "%s %d\n", playerName, Puan);  // Dosyaya kullanıcı adı ve puanı yazdır
    fclose(file);  // Dosyayı kapat
}

void showScores() {
    FILE* file = fopen("skorlar.txt", "r");  // "skorlar.txt" dosyasını okuma modunda aç

    if (file == NULL) {
        printf("Dosya acilamadi.\n");
        return;
    }

    char playerName[MAX_NAME_LENGTH];
    int score;

    gotoxy(2, 2);
    printf("\n--- Skor Tablosu ---\n");

    while (fscanf(file, "%s %d", playerName, &score) != EOF) {
        printf("%s - %d\n", playerName, score);
    }

    fclose(file);  // Dosyayı kapat
}

int main() {
    // Ses dosyasini arka planda oynatma
    PlaySound(TEXT("SnakeFon1.wav"), NULL, SND_FILENAME | SND_ASYNC);

    int Menusecim = 0;
    menu();
    scanf("%d", &Menusecim);

    for (int i = 1; i <= 21; i++) {
        gotoxy(1, i);
        printf("                              ");
    }

    switch(Menusecim){
	case 1:
	    {


    // Oyun ayarlarını yapma
    int n, s;
    int seviye;
    gotoxy(2, 14);
    printf("Zorluk seviyesi secin (1 - Basit, 2 - Normal, 3 - Zor): ");
    scanf("%d", &seviye);
    gotoxy(2, 2);
    printf("Puan: 0 ");
    gotoxy(2, 14);
    printf("                                                           ");

    // Yılan ve yem konumlarını belirleme
    fishing(&n, &s);

    // Oyun alanını sınırlar ile çevreleme
    sinirlar(0, 0, GENISLIK - 1, YUKSEKLIK - 1, 2);

    // Oyun döngüsü
    int sx = 40, sy = 12;
    int dy = 0, dx = 0;
    unsigned char tus;
    int a = 0, renk = 0;
    int devam = 0;

    int Puan;
    char playerName[MAX_NAME_LENGTH];

    // Müziği durdurma işlemi
    PlaySound(NULL, 0, 0);

    // Ses dosyasını arka planda oynatma
    PlaySound(TEXT("SnakeFon2.wav"), NULL, SND_FILENAME | SND_ASYNC);

    do {
        // Renk değiştirme
        switch (renk) {
            case 1:
                system("color 1f");
                break;
            case 2:
                system("color 2f");
                break;
            case 3:
                system("color 3f");
                break;
            case 4:
                system("color 4f");
                break;
            case 5:
                system("color 5f");
                renk = 0;
                break;
        }

        // Klavye girişini kontrol etme
        if (kbhit()) {
            tus = getch();
            if (tus == 224) {
                tus = getch();
                switch (tus) {
                    case 72:
                        dy = -1;
                        dx = 0;
                        break;
                    case 80:
                        dy = 1;
                        dx = 0;
                        break;
                    case 77:
                        dx = 1;
                        dy = 0;
                        break;
                    case 75:
                        dx = -1;
                        dy = 0;
                        break;
                }
            }
        }

        // Yılanın konumunu güncelleme
        sx = sx + dx;
        sy = sy + dy;
        if (sx >= GENISLIK - 1)
            sx = 1;
        if (sx <= 0)
            sx = GENISLIK - 2;
        if (sy >= YUKSEKLIK - 1)
            sy = 1;
        if (sy <= 0)
            sy = YUKSEKLIK - 2;

        // Yılanın konumunu ve görüntüsünü ekrana yazdırma
        snakeSutun[0] = sx;
        snakeSatir[0] = sy;
        int i;

        for (i = 1; i <= a; i++) {
            if ((sx == snakeSutun[i] && sy == snakeSatir[i])) {
                gotoxy(sx, sy);
                printf("-_-GAME-_-_-OVER-_-");
                getch();
                devam = 1;

                // Müziği durdurma işlemi
                PlaySound(NULL, 0, 0);
            }
        }

        gotoxy(sx, sy);
        printf("O");

        for (i = 0; i <= a; i++) {
            gotoxy(snakeSutun[i], snakeSatir[i]);
            printf("O");
        }

        // Yemi yenilen yere yerleştirme ve puanı güncelleme
        if (sx == n && sy == s) {
            fishing(&n, &s);
            a++;
            renk++;
            gotoxy(2, 2);
            printf("Puan: %d", a);
        }

        gotoxy(n, s);
        printf("+");

        // Yılanın hareket hızını ayarlama
        yilanHiziniAyarla(seviye);

        // Yılanın önceki konumunu temizleme
        gotoxy(sx, sy);
        printf(" ");

        for (i = 0; i <= a; i++) {
            gotoxy(snakeSutun[i], snakeSatir[i]);
            printf(" ");
        }

        // Yılanın parçalarını güncelleme
        for (i = a; i > 0; i--) {
            snakeSutun[i] = snakeSutun[i - 1];
            snakeSatir[i] = snakeSatir[i - 1];
        }

    } while (devam == 0);
    printf("                             ");
    // Oyuncu adını al ve puanı kaydet
    printf("\nOyun bitti! Puaninizi girin: ");
    scanf("%d", &Puan, a);

    printf("Kullanici adinizi girin: ");
    scanf(" %[^\n]s", playerName);

    if (strlen(playerName) == 0) {
        strcpy(playerName, "default");  // Kullanıcı adı girilmemişse "default" olarak ata
    }

    saveScore(playerName, Puan);

    break;
}

    case 2: {
            showScores();  // Skor tablosunu görüntüle
            break;
        }
        case 3: {
            exit(0);  // Programı sonlandır
            break;
        }
    }

    return 0;
}
