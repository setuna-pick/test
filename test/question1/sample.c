#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define N (50)          //読み込むファイルの１行の長さ
#define H (100)         //判断テーブルhan[]の最大要素数
struct HANDAN {
    char handan_name[N];
    int kaisuu;
    long time;
    bool count;
};

struct LOG {
    long time;
    char server_name[N];
    char result[N];
};

void printTime(long kikan)
{
    char kikan_second[14];
    sprintf(kikan_second, "%ld", kikan);
    int len = strlen(kikan_second);
    long sec, min, hour, day, month, year;
    year = kikan / 10000000000;
    month = (kikan / 1000000) % 100;
    day = (kikan / 10000) % 100;
    hour = (kikan / 100) % 100;
    min = kikan / 100;
    sec = kikan % 100;

    if (sec >= 60) {
        sec = sec - 60;
        min = min + 1;
    }
    if (min >= 60) {
        sec = min - 60;
        hour = hour + 1;
    }
    //printf("%ld年%ld月%ld日%ld時%ld分です\n",year,month,day,hour,min);
    //printf("kikan is %ld\n",kikan);

    if (len <= 2) {
        printf("故障期間は%ld秒です\n", sec);
    }
    else if (len <= 4) {
        printf("故障期間は%ld分%ld秒です\n", min, sec);
    }
    else if (len <= 6) {
        printf("故障期間は%ld時%ld分%ld秒です\n", hour, min, sec);
    }
    else if (len <= 8) {
        printf("故障期間は%ld日%ld時%ld分%ld秒です\n", day, hour, min,
            sec);
    }
    else if (len <= 10) {
        printf("故障期間は%ld月%ld日%ld時%ld分%ld秒です\n", month, day,
            hour, min, sec);
    }
    else if (len <= 14) {
        printf("故障期間は%ld年%ld月%ld日%ld時%ld分%ld秒です\n", year,
            month, day, hour, min, sec);
    }
}



int main()
{
    FILE* fp;
    struct HANDAN han[H];
    struct LOG log;
    char s[N], * cp;
    const char* sikiri = ",";
    long kikan;
    bool find;
    int i = 0, h = 0;
    int han_ctr = 0;                                //han[N]に登録した件数
    fp = fopen("server.txt", "r");

    while (fgets(s, N, fp) != NULL) {
        cp = strtok(s, sikiri);
        if (cp == NULL) continue;
        log.time = atol(cp);                    //ここで時間を挿入、文字列から数に変換
        cp = strtok(NULL, sikiri);
        if (cp == NULL) continue;
        strcpy(log.server_name, cp);            //ここでサーバーの名前を挿入
        cp = strtok(NULL, sikiri);
        if (cp == NULL) continue;
        strcpy(log.result, cp);                 //ここで結果を挿入
        if (strncmp(log.result, "-", 1) == 0) {     //1."-"かどうか
            find = false;
            for (h = 0; h < han_ctr; h++) {      //判断テーブル検索
                if (strcmp(log.server_name, han[h].handan_name) == 0) {
                    find = true;
                    break;
                }
            }
            //判断テーブル上にサーバーが存在しない場合
            if (find == false) {
                if (han_ctr >= H) {  //上限オーバーなら、次のログの読み込みへ
                    printf("判断テーブル上限:%d オーバー |%ld|%s|の故障監視をスキップ\n", H, log.time, log.server_name);
                    continue;
                }
                //判断テーブルに新規登録（値は初期値）
                h = han_ctr;
                strcpy(han[h].handan_name, log.server_name);
                han[h].count = false;
                han[h].time = 0;
                han[h].kaisuu = 0;
                han_ctr++;
            }
            han[h].kaisuu++;    //回数カウントアップ
            if (han[h].count == false) { //故障中でなければ、故障中にし、時刻を登録する。
                //故障中ならなにもしない。（回数カウントアップのみ）
                han[h].count = true;
                han[h].time = log.time;
            }
        }
        else {
            //"-"以外のケース
            for (h = 0; h < han_ctr; h++) {
                if (strcmp(log.server_name, han[h].handan_name) == 0) { //入力された値がどれかと一緒なら検索終了
                    if (han[h].count == true) {   //故障中なら故障状況をプリント
                        kikan = log.time - han[h].time;
                        printf("サーバー%sが故障状態です\n",
                            han[h].handan_name);
                        printf("log.time is %ld\nhandan is %ld\n",
                            log.time, han[h].time);
                        printTime(kikan);
                        han[h].count = false;
                    }
                    continue;                   //次のログの読み込みへ
                }
            }
        }
    }

    fclose(fp);
    return 0;
}