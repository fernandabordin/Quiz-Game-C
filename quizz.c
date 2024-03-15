#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "printAsciiArt.h"

#define INTRO "telaInicial.txt"
#define MENU "menu.txt"
#define SCORE "placarTotal.txt"
#define EXIT "telaFinal.txt"
#define EASY_QUESTIONS "dificuldadeFacil.txt"
#define MEDIUM_QUESTIONS "dificuldadeMedia.txt"
#define HARD_QUESTIONS "dificuldadeDificil.txt"
#define QUESTION_1 "pergunta1.txt"
#define QUESTION_2 "pergunta2.txt"
#define QUESTION_3 "pergunta3.txt"
#define QUESTION_4 "pergunta4.txt"
#define QUESTION_5 "pergunta5.txt"
#define SELECT_DIFFICULTY "escolhaDificuldade.txt"
#define EASY_QUESTIONS_FILE "easy.txt"
#define MEDIUM_QUESTIONS_FILE "medium.txt"
#define HARD_QUESTIONS_FILE "hard.txt"
#define ALL_QUESTIONS 10
#define N_QUESTIONS 5
#define L_QUESTIONS 128
#define DELAY 1500

typedef enum {
    STATE_INITIAL,
    STATE_MENU,
    STATE_SELECT_DIFFICULTY,
    STATE_PLAYING,
    STATE_SCORE
} State; 

typedef enum {
    EASY,
    MEDIUM,
    HARD
} Difficulty;

struct Questions {
    char pergunta[L_QUESTIONS];
    char respostaCerta[L_QUESTIONS];
    char r1[L_QUESTIONS];
    char r2[L_QUESTIONS];
    char r3[L_QUESTIONS];
    char r4[L_QUESTIONS];
    char r5[L_QUESTIONS];
};

void delay(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

void printIntro() {
    printAsciiArt(INTRO);
}

void printMenu() {
    system("cls");
    printAsciiArt(MENU);
}

void printDifficulty() {
    system("cls");
    printAsciiArt(SELECT_DIFFICULTY);
}

struct Questions todasPerguntas[ALL_QUESTIONS];
struct Questions perguntas[N_QUESTIONS];
State estadoAtual;
Difficulty dificuldadeAtual;
int perguntaAtual = 0;
int pontuacao = 0;
int randomNumbers[5];

void generateRandomNumbers() {
    time_t t;
    int count = 0;
	srand((unsigned) time(&t));
 
	while (count < 5) { 
		int randNum = rand() % 10;
		bool found = false;
		 
		for (int i = 0; i < 10; i++) { 
			if(randomNumbers[i] == randNum) { 
				found = true; 
				break;
			}
        }

        if(!found) { 
                randomNumbers[count] = randNum; 
                count++; 
            } 
	} 
}

void printQuestions() {
    system("cls");
    if (perguntaAtual == 0) printAsciiArt(QUESTION_1);
    if (perguntaAtual == 1) printAsciiArt(QUESTION_2);
    if (perguntaAtual == 2) printAsciiArt(QUESTION_3);
    if (perguntaAtual == 3) printAsciiArt(QUESTION_4);
    if (perguntaAtual == 4) printAsciiArt(QUESTION_5);
    printf("\n");
    printf("%s", perguntas[perguntaAtual].pergunta);
    printf("%s", perguntas[perguntaAtual].r1);
    printf("%s", perguntas[perguntaAtual].r2);
    printf("%s", perguntas[perguntaAtual].r3);
    printf("%s", perguntas[perguntaAtual].r4);
    printf("%s", perguntas[perguntaAtual].r5);
}

void printAnswer(bool isRight) {
    char ans = perguntas[perguntaAtual].respostaCerta[0];
    printf("\n");
    if (isRight) printf("Parabens, voce acertou!\n");
    else {
        switch (ans) {
            case '1':
                printf("Que pena, voce errou, a resposta correta era: %s\n", perguntas[perguntaAtual].r1);
                break;

            case '2':
                printf("Que pena, voce errou, a resposta correta era: %s\n", perguntas[perguntaAtual].r2);
                break;

            case '3':
                printf("Que pena, voce errou, a resposta correta era: %s\n", perguntas[perguntaAtual].r3);
                break;

            case '4':
                printf("Que pena, voce errou, a resposta correta era: %s\n", perguntas[perguntaAtual].r4);
                break;

            case '5':
                printf("Que pena, voce errou, a resposta correta era: %s\n", perguntas[perguntaAtual].r5);
                break;
        }
    }
}

void printScore() {
    system("cls");
    printAsciiArt(SCORE);
    printf("\nSua pontuacao e: %d\n\n", pontuacao);
    printf("Digite 1 para jogar novamente ou espaco para sair\n");
}

void initializeGame() {
    perguntaAtual = 0;
    pontuacao = 0;

    int i;
    for (i = 0; i < 5 ; i++) {
        randomNumbers[i] = -1;
    }
}

void openQuestionFile(char filename[]) {
    FILE *f = fopen(filename, "r");
    char line[L_QUESTIONS];
    size_t read;
    int lineCounter = 0;
    int i = 0;

    if (f == NULL) {
        printf("File not found.\n");
        exit(0);
    }

    while (i < ALL_QUESTIONS) {
        lineCounter++;
        fgets(todasPerguntas[i].pergunta, sizeof(todasPerguntas[i].pergunta), f);
        fgets(todasPerguntas[i].r1, sizeof(todasPerguntas[i].r1), f);
        fgets(todasPerguntas[i].r2, sizeof(todasPerguntas[i].r2), f);
        fgets(todasPerguntas[i].r3, sizeof(todasPerguntas[i].r3), f);
        fgets(todasPerguntas[i].r4, sizeof(todasPerguntas[i].r4), f);
        fgets(todasPerguntas[i].r5, sizeof(todasPerguntas[i].r5), f);
        fgets(todasPerguntas[i].respostaCerta, sizeof(todasPerguntas[i].respostaCerta), f);
        i++;
    }

    for (i = 0; i < N_QUESTIONS; i++) {
        perguntas[i] = todasPerguntas[randomNumbers[i]];
    }

    fclose(f);
}

void getQuestions() {
    generateRandomNumbers();
    switch (dificuldadeAtual) {
        case EASY:
            openQuestionFile(EASY_QUESTIONS_FILE);
            break;

        case MEDIUM:
            openQuestionFile(MEDIUM_QUESTIONS_FILE);
            break;

        case HARD:
            openQuestionFile(HARD_QUESTIONS_FILE);
            break;
    }
}

void printExit() {
    system("cls");
    printAsciiArt(EXIT);
    exit(0);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    char option;

    estadoAtual = STATE_INITIAL;

    initializeGame();

    while(1) {
        switch(estadoAtual) {
            case STATE_INITIAL:
                printIntro();
                delay(DELAY);
                estadoAtual = STATE_MENU;
                break;

            case STATE_MENU:
                initializeGame();
                printMenu();
                option = getch();

                if (option == ' ') printExit();
                else estadoAtual = STATE_SELECT_DIFFICULTY;

            case STATE_SELECT_DIFFICULTY:
                printDifficulty();
                option = getch();

                system("cls");
                switch (option) {
                    case '1':
                        dificuldadeAtual = EASY;
                        estadoAtual = STATE_PLAYING;
                        printAsciiArt(EASY_QUESTIONS);
                        getQuestions();
                        break;

                    case '2':
                        dificuldadeAtual = MEDIUM;
                        estadoAtual = STATE_PLAYING;
                        printAsciiArt(MEDIUM_QUESTIONS);
                        getQuestions();
                        break;

                    case '3':
                        dificuldadeAtual = HARD;
                        estadoAtual = STATE_PLAYING;
                        printAsciiArt(HARD_QUESTIONS);
                        getQuestions();
                        break;

                    case ' ':
                        printExit();
                        break;

                    default:
                        printf("Opcao invalida!\n");
                        
                }
                break;

            case STATE_PLAYING:
                delay(DELAY);
                printQuestions();

                option = getch();
                if (option == perguntas[perguntaAtual].respostaCerta[0]) { 
                    printAnswer(true);

                    switch (dificuldadeAtual) {
                        case EASY:
                            pontuacao += 5;
                            break;
                        
                        case MEDIUM:
                            pontuacao += 10;
                            break;
                        
                        case HARD:
                            pontuacao += 15;
                            break;
                        }
                }
                else printAnswer(false);

                perguntaAtual++;

                if (perguntaAtual == 5) estadoAtual = STATE_SCORE;
                break;

            case STATE_SCORE:
                delay(DELAY);
                printScore();

                option = getch();

                switch (option) {
                    case '1':
                        estadoAtual = STATE_MENU;
                        initializeGame();
                        break;

                    case ' ':
                        printExit();
                        break;

                    default:
                        printf("Opcao invalida!\n");
                }
            break;
        }
    }
    return 0;
}