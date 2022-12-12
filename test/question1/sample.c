#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define N (50)          //�ǂݍ��ރt�@�C���̂P�s�̒���
#define H (100)         //���f�e�[�u��han[]�̍ő�v�f��
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
    //printf("%ld�N%ld��%ld��%ld��%ld���ł�\n",year,month,day,hour,min);
    //printf("kikan is %ld\n",kikan);

    if (len <= 2) {
        printf("�̏���Ԃ�%ld�b�ł�\n", sec);
    }
    else if (len <= 4) {
        printf("�̏���Ԃ�%ld��%ld�b�ł�\n", min, sec);
    }
    else if (len <= 6) {
        printf("�̏���Ԃ�%ld��%ld��%ld�b�ł�\n", hour, min, sec);
    }
    else if (len <= 8) {
        printf("�̏���Ԃ�%ld��%ld��%ld��%ld�b�ł�\n", day, hour, min,
            sec);
    }
    else if (len <= 10) {
        printf("�̏���Ԃ�%ld��%ld��%ld��%ld��%ld�b�ł�\n", month, day,
            hour, min, sec);
    }
    else if (len <= 14) {
        printf("�̏���Ԃ�%ld�N%ld��%ld��%ld��%ld��%ld�b�ł�\n", year,
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
    int han_ctr = 0;                                //han[N]�ɓo�^��������
    fp = fopen("server.txt", "r");

    while (fgets(s, N, fp) != NULL) {
        cp = strtok(s, sikiri);
        if (cp == NULL) continue;
        log.time = atol(cp);                    //�����Ŏ��Ԃ�}���A�����񂩂琔�ɕϊ�
        cp = strtok(NULL, sikiri);
        if (cp == NULL) continue;
        strcpy(log.server_name, cp);            //�����ŃT�[�o�[�̖��O��}��
        cp = strtok(NULL, sikiri);
        if (cp == NULL) continue;
        strcpy(log.result, cp);                 //�����Ō��ʂ�}��
        if (strncmp(log.result, "-", 1) == 0) {     //1."-"���ǂ���
            find = false;
            for (h = 0; h < han_ctr; h++) {      //���f�e�[�u������
                if (strcmp(log.server_name, han[h].handan_name) == 0) {
                    find = true;
                    break;
                }
            }
            //���f�e�[�u����ɃT�[�o�[�����݂��Ȃ��ꍇ
            if (find == false) {
                if (han_ctr >= H) {  //����I�[�o�[�Ȃ�A���̃��O�̓ǂݍ��݂�
                    printf("���f�e�[�u�����:%d �I�[�o�[ |%ld|%s|�̌̏�Ď����X�L�b�v\n", H, log.time, log.server_name);
                    continue;
                }
                //���f�e�[�u���ɐV�K�o�^�i�l�͏����l�j
                h = han_ctr;
                strcpy(han[h].handan_name, log.server_name);
                han[h].count = false;
                han[h].time = 0;
                han[h].kaisuu = 0;
                han_ctr++;
            }
            han[h].kaisuu++;    //�񐔃J�E���g�A�b�v
            if (han[h].count == false) { //�̏ᒆ�łȂ���΁A�̏ᒆ�ɂ��A������o�^����B
                //�̏ᒆ�Ȃ�Ȃɂ����Ȃ��B�i�񐔃J�E���g�A�b�v�̂݁j
                han[h].count = true;
                han[h].time = log.time;
            }
        }
        else {
            //"-"�ȊO�̃P�[�X
            for (h = 0; h < han_ctr; h++) {
                if (strcmp(log.server_name, han[h].handan_name) == 0) { //���͂��ꂽ�l���ǂꂩ�ƈꏏ�Ȃ猟���I��
                    if (han[h].count == true) {   //�̏ᒆ�Ȃ�̏�󋵂��v�����g
                        kikan = log.time - han[h].time;
                        printf("�T�[�o�[%s���̏��Ԃł�\n",
                            han[h].handan_name);
                        printf("log.time is %ld\nhandan is %ld\n",
                            log.time, han[h].time);
                        printTime(kikan);
                        han[h].count = false;
                    }
                    continue;                   //���̃��O�̓ǂݍ��݂�
                }
            }
        }
    }

    fclose(fp);
    return 0;
}