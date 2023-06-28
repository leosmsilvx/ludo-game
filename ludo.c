#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

typedef struct jogador {
    char nome[255];
    char cor[20];    
    int casaInicial;
} Jogador;

typedef struct cor {
    char cor[20];
    bool escolhido;    
    int casaInicial;
} Cor;

typedef struct peca {
    int num; // 1-4 em relação ao jogador
    int numJogador; //1, 2, 3, 4
    int casa; // Posicao do array 0-51 - 999 fora do tabuleiro 
    int contagem; // Numero de casas andadas 1-51
    int contagemFinal; // Numero de casas para ganhar
} Peca;

typedef struct casa {
    Peca pecas[16];
    bool protegida;
} Casa;

int rodarDado();

int main(){
    srand(time(NULL));

    Casa casas[52];    
    //Protegidas = 1, 9, 14, 22, 27, 35, 40, 48 
    //Coloridas = 1(Amarelo), 14(Azul), 27(Vermelho), 40(Verde)
    for(int i = 0; i < 52; i++){
        if(i == 1 || i == 9 || i == 14 || i == 22 || i == 27 || i == 35 || i == 40 || i == 48){        
            casas[i].protegida = true;   
        } else {            
            casas[i].protegida = false;
        }
    }

    Cor cores[4] = {
        {cor: "Vermelho", escolhido: 0, casaInicial: 1},
        {cor: "Azul", escolhido: 0, casaInicial: 14},
        {cor: "Amarelo", escolhido: 0, casaInicial: 27},
        {cor: "Verde", escolhido: 0, casaInicial: 40},
    };
    int numJogadores = 0;

    bool validNum = false;
    while(!validNum){
        printf("Digite o numero de jogadores (2 ou 4): ");
        scanf("%d", &numJogadores);
        if(numJogadores == 2 || numJogadores == 4){
            validNum = true;
        }
    }

    Jogador jogadores[4];
    Peca pecas[16];
    for(int i = 0; i < numJogadores; i++){
        int cor;
        printf("Escolha uma cor para o jogador %d\n", i+1);

        for(int j = 0; j < 4; j++){
            pecas[i*4+j].num = j+1;
            pecas[i*4+j].numJogador = i;
            pecas[i*4+j].casa = 999; // Fora do tabuleiro
            pecas[i*4+j].contagem = 0;            
            pecas[i*4+j].contagemFinal = 999; // Não iniciou a contagem final

            if(cores[j].escolhido != 1){
                printf("- %s(%d)\n", cores[j].cor, j+1);
            }
        }
        bool validCor = false;
        while(!validCor){
            scanf("%d", &cor);
            if(cor < 1 || cor > 4){
                printf("Digite uma cor valida: ");
            } else if(!cores[cor-1].escolhido){
                cores[cor-1].escolhido = true;
                validCor = true;
                strcpy(jogadores[i].cor, cores[cor-1].cor);
                jogadores[i].casaInicial = cores[cor-1].casaInicial;
            } else {
                printf("Digite uma cor valida: ");
            }
        }

        printf("\nDigite o nome do jogador %s: ", jogadores[i].cor);
        setbuf(stdin, NULL);
        scanf("%s", &jogadores[i].nome);
    }

    bool ganhador = false;
    while(!ganhador){
        for(int i = 0; i < numJogadores; i++){
            bool validTresSeis = false;            
            int cont = 0;
            int valorDado;
            int dadosJogados[2];
            while(!validTresSeis){
                cont++;                
                if(cont == 4){
                    printf("Você tirou 6 tres vezes, perdeu a vez!\n");
                    cont = 0;
                    break;
                }
                char dado;
                bool digitouDado = false;
                printf("\n%s rode o dado!\nPara rodar digite 'd': ", jogadores[i].nome);            
                while(!digitouDado){  
                    setbuf(stdin, NULL);  
                    scanf("%c", &dado);
                    if(dado == 'd' || dado == 'D'){
                        digitouDado = true;
                    }
                }
                
                valorDado = rodarDado();  
                dadosJogados[cont-1] = valorDado;
                if(valorDado != 6){
                    validTresSeis = true;
                }
            }
            
            for(int k = 0; k < cont; k++){
                int pecasPossiveis[4] = {0, 0, 0, 0};
                if(dadosJogados[k] == 6){
                    //Tirou 6 no dado
                    for(int j = 0; j < 4; j++){
                        if(pecas[i*4+j].contagemFinal == 999){
                            pecasPossiveis[j] = 1;
                        } else {
                            if(pecas[i*4+j].contagemFinal == dadosJogados[k]){                                
                                pecasPossiveis[j] = 1;
                            }
                        }
                    }
                } else {
                    //Não tirou 6 no dado
                    for(int j = 0; j < 4; j++){
                        if(pecas[i*4+j].casa != 999){                            
                            //Peça está no tabuleiro
                            if(pecas[i*4+j].contagemFinal == 999){
                                pecasPossiveis[j] = 1;
                            } else {
                                if(pecas[i*4+j].contagemFinal == dadosJogados[k]){                                
                                    pecasPossiveis[j] = 1;
                                }
                            }
                        }
                    }
                }
                
                
        
                bool temPecasPossiveis = false;
                for(int j = 0; j < 4; j++){    
                    if(pecasPossiveis[j] == 1){    
                        temPecasPossiveis = true;             
                    }
                } 

                if(temPecasPossiveis){
                    printf("Pecas possiveis:\n");
                    for(int j = 0; j < 4; j++){    
                        if(pecasPossiveis[j] == 1){    
                            temPecasPossiveis = true;
                            char textoCasa[255];
                            char textoContagem[255];
                            if(pecas[i*4+j].casa == 999){
                                //Muda o texto para explicar que a peça está fora do tabuleiro
                                strcpy(textoCasa, "Inicial");
                                strcpy(textoContagem, "Fora do tabuleiro");
                            } else {                               
                                //Muda o texto para a posição em que ele está no tabuleiro
                                //array, texto igual o de um printf, variavel para converter trocar no %d
                                sprintf(textoCasa, "%d", pecas[i*4+j].casa);                                
                                sprintf(textoContagem, "%d", pecas[i*4+j].contagem);
                            }
                            printf("Peca%d (%d) - Posicao: %s - Contagem: %s\n", j+1, j+1, textoCasa, textoContagem);
                        }
                    } 

                    bool validPeca = false;
                    int pecaParaMover;
                    while(!validPeca){
                        printf("\nDigite a peca que você quer mover: ");
                        scanf("%d", &pecaParaMover);
                        if(pecasPossiveis[pecaParaMover-1] == 1){
                            validPeca = true;
                            int j = pecaParaMover - 1;
                            if(pecas[i*4+j].contagemFinal == 999){
                                if(pecas[i*4+j].casa == 999){
                                    //Peça esta na posição inicial
                                    pecas[i*4+j].casa = jogadores[i].casaInicial;
                                } else {
                                    pecas[i*4+j].contagem += dadosJogados[k];
                                    if(pecas[i*4+j].casa + dadosJogados[k] >= 51){                                    
                                        //Peça move no tabuleiro porem ela completa a volta
                                        int novaCasa = pecas[i*4+j].casa + dadosJogados[k] - 51;
                                        pecas[i*4+j].casa = novaCasa;
                                    } else {    
                                        //Peça move no tabuleiro                                
                                        pecas[i*4+j].casa += dadosJogados[k];
                                    }
                                }  

                                //Logica comer a peça
                                for(int w = 0; w < numJogadores*4; w++){
                                    if(pecas[i*4+j].casa == pecas[w].casa){
                                        if(pecas[i*4+j].numJogador != pecas[w].numJogador){
                                            if(!casas[pecas[i*4+j].casa].protegida){                                                
                                                pecas[w].casa = 999;
                                                pecas[w].contagem = 0;
                                                printf("\n!!!!\n%s comeu a peca %d de %s hahaha!\n!!!!\n\n", jogadores[pecas[i*4+j].numJogador].nome, pecas[w].num, jogadores[pecas[w].numJogador].nome);
                                            }
                                        }
                                    }
                                }

                                //Logica ganhar
                                if(pecas[i*4+j].contagem > 51){
                                    pecas[i*4+j].contagemFinal = 6 - (pecas[i*4+j].contagem - 51);
                                }

                            } else {
                                if(pecas[i*4+j].contagemFinal == dadosJogados[k]){
                                    ganhador = true;
                                    printf("Parabens %s!!!\nVoce ganhou o jogo :)", jogadores[i].nome);
                                    return 0;
                                }
                                
                            }
                            
                        }
                    }
                } else {
                    printf("%s nao possui pecas possiveis :(\nProximo!\n", jogadores[i].nome);
                }
            }
        }
    }

    return 0;
}

int rodarDado(){   
    int r = (rand() % 6) + 1;
    printf("\nDado caiu em: %d\n", r);
    return r;
}