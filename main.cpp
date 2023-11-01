// versao funcionando


#include "mbed.h"
#include "Arduino.h"
#include <MCUFRIEND_kbv.h>
#include <cstdio> 
#include "pipe.h"
#define velo 0.0015
DigitalOut LED(PA_5); // é o LED (LED1) na placa NUCLEO-F103RB
DigitalIn botao(PC_13); // botão-do-usuário na placa NUCLEO-F103RB
Serial pc(USBTX, USBRX); // declara o objeto pc para comunicação serial nativo
InterruptIn botao1(PB_13, PullUp);
AnalogIn xAxis(PC_5);
AnalogIn yAxis(PC_4);
DigitalOut Enable(PB_1); //liga o motor
DigitalOut Enable_x(PB_2); //liga o motor
DigitalOut Sentido(PB_15);
DigitalOut Sentido_x(PB_12);
DigitalOut Passos(PB_14);
DigitalOut Passos_x(PB_11);
DigitalOut rele1(PC_2);
InterruptIn estadoemergencia(PC_8);
DigitalOut rele2(PC_3);
MCUFRIEND_kbv tft;
Timer debounce; 

uint8_t Orientation =1;
int larg=tft.width();
int altura=tft.height();

//////////DEFINIÇÂO DE CORES/////////////////////

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define GRAY 0x8410
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define RED 0xF800

//tela 0 vai ser a quantidade para pipetagem


int tela0=0;int tela1=1; int tela2=0;; int tela3=0; int tela4=0; int tela5=0;//vai verificar em que tela se está
int fixo_0=1;int fixo_1=1; int fixo_2=1; int fixo_3=1; int fixo_4=1; int fixo_5=1;//vai permitir que a tela só passe uma única vez 
int x,y;
int lado_tela0=0;int lado_tela1=0; int lado_tela2=0; int lado_tela3=0; int lado_tela5=0;//permite verificar o botao que está sendo pressionado
int teste_y=1; int teste_y_2=1; int teste_x_2=1; int teste_x_3=1; int teste_x_0=1; int teste_x_5=1; int teste_y_1=0;
int vol=1; int muda=0; int libera=0; int letras_tela1=0;
int salva_x=0; int salva_y=0; int salva_z=0;
int lista_x[10]={0,0,0,0,0,0,0,0,0,0}; int lista_y[10]={0,0,0,0,0,0,0,0,0,0}; int lista_z[10]; int lista_vol[10]={0,0,0,0,0,0,0,0,0,0}; int index=0; int estado=0;
int pos_atual_y; int contador_y=0;int flag_jogManual = 1;int valor_final_y=10000; int valor_inicial_y=0; int sensibilidade=2000; int pos_atual_x; int contador_x=0; int valor_final_x=999; int valor_inicial_x=0;
int volume_inicial=0; int volume_final=10; int fixo_1_mov=0; int fixo_1_mov_2=0;
int botao_teste =0; int botao_teste_tela0=0; int botao_teste_tela2=0; int botao_teste_tela3=0; int botao_teste_tela4=0; int botao_teste_tela5=0;int sensibilidade_x=1500;
int variavel_pipeta=0; int index_pipeta=0; int fixo_emergencia=1;
bool estadoBE = 0; bool bubu =0;
void tela(){
    tft.fillRoundRect(2,2, altura-4,larg-4,0, WHITE);
    tft.fillRoundRect(6,6,altura-12,larg-12,0, GRAY);
    tft.fillRoundRect(15, 13, altura-30, 45,0, WHITE);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(90,28);
    tft.println("PIPETAGEM");
    tft.drawRGBBitmap(220,16,pipeta,40,40);
    tft.fillRoundRect(larg-120, 80, 100, 40, 15, WHITE); //botao
    tft.fillRoundRect(larg-120, 140, 100, 40, 15, WHITE);
    tft.setCursor(larg-110,90);
    tft.println("Posicao");
    tft.setCursor(larg-110,150);
    tft.println("Memoria");
    }

void mov_tela1(){
    y=yAxis.read()*1000;
    if(y<300){
        if(lado_tela1!=1){
        y=yAxis.read()*1000;
        tft.fillRoundRect(larg-120, 80, 100, 40, 15, CYAN);
        tft.fillRoundRect(larg-120, 140, 100, 40, 15, WHITE);
        lado_tela1=1;
        tft.setCursor(larg-110,90);
        tft.println("Posicao");
        tft.setCursor(larg-110,150);
        tft.println("Memoria");
        wait_ms(200);}
        } 

    if(y>800){
        if(lado_tela1!=2){
        y=yAxis.read()*1000;
        tft.fillRoundRect(larg-120, 80, 100, 40, 15, WHITE);
        tft.fillRoundRect(larg-120, 140, 100, 40, 15, CYAN);
        tft.setCursor(larg-110,90);
        tft.println("Posicao");  
        tft.setCursor(larg-110,150);
        tft.println("Memoria");
        wait_ms(200);
        lado_tela1=2;}
        }
    }


void sensi(){
    tft.fillScreen(GRAY);
    tft.fillRoundRect(2,2, altura-4,larg-4,0, WHITE);
    tft.fillRoundRect(6,6,altura-12,larg-12,0, GRAY);
    tft.fillRoundRect(10, 15,35, 45, 0, WHITE);
    tft.fillRoundRect(60, 15,45, 45, 0, WHITE);
    tft.fillRoundRect(110, 15,35, 45, 0, WHITE);
    tft.fillRoundRect(160, 15,45, 45, 0, WHITE);
    tft.fillRoundRect(210, 15,35, 45, 0, WHITE);
    tft.fillRoundRect(260, 15,45, 45, 0, WHITE);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(22,30);
    tft.println("X :");
    tft.setCursor(122,30);
    tft.println("Y :");
    tft.setCursor(222,30);
    tft.println("Z :");
    tft.fillRoundRect(35, 100, 120, 50, 5,WHITE);
    tft.fillRoundRect(170, 100, 120, 50, 5,WHITE);
    tft.fillRoundRect(103, 165, 120, 50, 5,WHITE);
    tft.fillRoundRect(30,185,40,40,5, WHITE);
    tft.setCursor(42,200);
    tft.println("<");
    tft.setCursor(70,115);
    tft.println("MAPA");
    tft.setCursor(195,115);
    tft.println("MANUAL");
    tft.setCursor(120,180);
    tft.println("DEFINIR");
    }

void manual(){
        tft.fillScreen(GRAY);
        tft.fillRoundRect(2,2, altura-4,larg-4,0, WHITE);
        tft.fillRoundRect(6,6,altura-12,larg-12,0, GRAY);
        tft.fillRoundRect(15, 13, altura-30, 45,0, WHITE);
        tft.setTextSize(2);
        tft.setTextColor(BLACK);
        tft.setCursor(70,28);
        tft.println("CONTROLE MANUAL");
        tft.fillRoundRect(30,185,40,40,5, CYAN);
        tft.fillRoundRect(30,100,60,60,5, CYAN);
        tft.setCursor(50,130);
        tft.println("J");
        tft.setCursor(42,200);
        tft.println("<");
    }


void volume(){
        tft.fillScreen(GRAY);
        tft.fillRoundRect(2,2, altura-4,larg-4,0, WHITE);
        tft.fillRoundRect(6,6,altura-12,larg-12,0, GRAY);
        tft.fillRoundRect(15, 13, altura-30, 45,0, WHITE);
        tft.setTextSize(2);
        tft.setTextColor(BLACK);
        tft.setCursor(130,28);
        tft.println("VOLUME");
        tft.fillRoundRect(140,100,60,60,5, CYAN);
        tft.fillRoundRect(30,185,40,40,5, WHITE);
        tft.fillRoundRect(250,185,40,40,5, WHITE);
        tft.setCursor(42,200);
        tft.println("<");

    }


void mapa(){
        tft.fillScreen(GRAY);
        tft.fillRoundRect(2,2, altura-4,larg-4,0, WHITE);
        tft.fillRoundRect(6,6,altura-12,larg-12,0, GRAY);
        tft.fillRoundRect(15, 13, altura-30, 45,0, WHITE);
        tft.setTextSize(2);
        tft.setTextColor(BLACK);
        tft.setCursor(50,28);
        tft.println("MAPA DE COORDENADAS");
        tft.fillRoundRect(30,185,40,40,5, CYAN);    
        tft.setCursor(42,200);
        tft.println("<");
    }


void historico(){
        tft.fillScreen(GRAY);
        tft.fillRoundRect(2,2, altura-4,larg-4,0, WHITE);
        tft.fillRoundRect(6,6,altura-12,larg-12,0, GRAY);
        tft.fillRoundRect(30, 10, 260, 25,0, WHITE);
        tft.setTextSize(2);
        tft.setTextColor(BLACK);
        tft.setCursor(55,14);
        tft.println("X");
        tft.setCursor(110,14);
        tft.println("Y");
        tft.setCursor(165,14);
        tft.println("Z");
        tft.setCursor(220,14);
        tft.println("VOL");
        tft.fillRoundRect(10, 39, 145, 25,0, WHITE); 
        tft.fillRoundRect(10, 69, 145, 25,0, WHITE); 
        tft.fillRoundRect(10, 99, 145, 25,0, WHITE);
        tft.fillRoundRect(10, 129, 145, 25,0, WHITE); 
        tft.fillRoundRect(10, 159, 145, 25,0, WHITE);
        tft.fillRoundRect(165, 39, 145, 25,0, WHITE); 
        tft.fillRoundRect(165, 69, 145, 25,0, WHITE); 
        tft.fillRoundRect(165, 99, 145, 25,0, WHITE);
        tft.fillRoundRect(165, 129, 145, 25,0, WHITE); 
        tft.fillRoundRect(165, 159, 145, 25,0, WHITE);
        tft.fillRoundRect(30,190,40,40,5,WHITE);
        tft.fillRoundRect(110,190,60,40,5,WHITE);
        tft.fillRoundRect(190,190,100,40,5,WHITE);
        tft.setCursor(200,200);
        tft.println("pipetar");
        tft.setCursor(120,200);
        tft.println("nova");
        
        }


void emergencia(){
    tft.fillRoundRect(2,2, altura-4,larg-4,0, WHITE);
    tft.fillRoundRect(6,6,altura-12,larg-12,0, GRAY);
    tft.fillRoundRect(15, 13, altura-30, 45,0, RED);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.setCursor(90,28);
    tft.println("EMERGENCIA");
    tft.fillRoundRect(100, 70, 120,130,0, WHITE);
    tft.drawRGBBitmap(110,80,kaique,100,107);
}

void jogManual(){
    y=yAxis.read()*1000;
    x=xAxis.read()*1000;
    if(y>750){
        y=yAxis.read()*1000;
        if(contador_y>=valor_final_y){
            contador_y=valor_final_y;
            Enable=1;}
        else{
        Enable=0;
        Sentido=0;
        Passos=1;
        //if(y<400 & y>200){sensibilidade=(0.015*y-2)*1000;}
        //if(y<10){sensibilidade=2000;}
        wait_us(sensibilidade);
        Passos=0;
        contador_y+=1;}
    }
    if(y<300){
        y=yAxis.read()*1000;
        if(contador_y<=valor_inicial_y){
            contador_y=valor_inicial_y;
            Enable=1;}
        else{
        Enable=0;
        Sentido=1;
        Passos=1;
        //if(y>500 & y<800){sensibilidade=5000;}
        //if(y>801){sensibilidade=2000;}
        wait_us(sensibilidade);
        Passos=0;
        contador_y-=1;}}
    if(y<750 && y>300){Enable=1;}
    if(x<300){
        x=xAxis.read()*1000;
        if(contador_x<=valor_inicial_x){
            contador_x=valor_final_x;
            Enable_x=1;}
        else {
        Enable_x=0;
        Sentido_x=1;
        Passos_x=1;
        wait_us(sensibilidade_x);
        Passos_x=0;

        contador_x-=1;}}
    
    if(x>800){
        x=xAxis.read()*1000;
        if(contador_x>=valor_final_x){
            Enable_x=1;}
        else{
            Enable_x=0;
            Sentido_x=0;
            Passos_x=1;
            wait_us(sensibilidade_x);
            Passos_x=0;
            contador_x+=1;}}

    if(x<800 && x>300){Enable_x=1;}
        
        }


void toggle_emergencia1(void){
    estadoBE=1;}
void toggle_emergencia2(void){
    estadoBE=0;}

void toggle(void);
void toggle(){
    
    if(debounce.read_ms()>50){
    if (tela3==1 && lado_tela3==1){libera=!libera;}
    if(tela1==1 && lado_tela1==1){botao_teste=1;}
    if(tela0==1 && lado_tela0==1){botao_teste_tela0=1;}
    if(tela0==1 && lado_tela0==2){botao_teste_tela0=2;}
    if(tela2==1 && lado_tela2==1){ botao_teste_tela2=1;}
    if(tela2==1 && lado_tela2==2){botao_teste_tela2=2;}
    if(tela2==1 && lado_tela2==3){botao_teste_tela2=3;}
    if(tela2==1 && lado_tela2==4){botao_teste_tela2=4;}
    if(tela3==1 && lado_tela3==2){botao_teste_tela3=1;}
    if(tela4==1){botao_teste_tela4=1;}
    if(tela5==1 && lado_tela5==1){botao_teste_tela5=1;}
    if(tela5==1 && lado_tela5==2){botao_teste_tela5=2;}
    if(tela5==1 && lado_tela5==3){botao_teste_tela5=3;}
    }
    debounce.reset();
     }


void volumes(){
    y=yAxis.read()*1000;
    if(y<300){
        if(vol>=volume_final){
            vol=volume_final;}
        else{

        vol=vol+1;
        
        tft.fillRoundRect(140,100,60,60,5, CYAN);tft.setCursor(165,120);tft.println(vol);fixo_0=0;
        wait_ms(150);}}
    if(y>900){
        if(vol<=volume_inicial){
            vol=volume_inicial;}
        else{
           
            vol=vol-1;
           
            tft.fillRoundRect(140,100,60,60,5, CYAN);tft.setCursor(165,120);tft.println(vol);fixo_0=0;
            wait_ms(150);}}
}

void pipetar(void){
    rele1=0;
    rele2=0;
    wait(0.1);
    rele2=1;
    rele1=1;

}


void teste_pipeta(void){

    while(index_pipeta<9){
    if (variavel_pipeta<lista_vol[index_pipeta]){
        wait(5); //pega
        pipetar();
        wait(5); //solta[i]
        pipetar();
        variavel_pipeta+=1;}
    else{
        variavel_pipeta=0;
        index_pipeta+=1;}
    }
}


void principal(void){
    botao1.fall(&toggle);
    estadoemergencia.fall(&toggle_emergencia1);
    estadoemergencia.rise(&toggle_emergencia2);
    debounce.start();
    while(1){
        bubu=1;
        rele1=1;
        rele2=1;
        Enable=1;
        Enable_x=1;
        Sentido=0;
        y=yAxis.read()*1000;
        x=xAxis.read()*1000;
        
         //estadoBE = estadoemergencia.read();
        //printf("\rY=%4d",y);
        //printf("\rx=%4d",x);
        if(estadoBE==0){
            fixo_emergencia=1;
        /////////////////////////TELA DE INICIO//////////////////////////////////////////// 
        if (tela1==1){
                if(fixo_1==1){tela();fixo_1=0;}
                mov_tela1();
                if (botao_teste==1){tela1=0;fixo_0=1;tela0=1;lado_tela1=0;botao_teste=0;}}


        //////////////TELA DE VOLUME//////////////////////////////////////////////////////////////
        if(tela0==1 ){
            if(fixo_0==1){volume();tft.fillRoundRect(140,100,60,60,5, CYAN);tft.setCursor(160,120);tft.println(vol);fixo_0=0;}
            volumes();

            if (x<50 && teste_x_0>0){wait_ms(150);teste_x_0=teste_x_0-1;}
            if(x>950 && teste_x_0<2){wait_ms(150);teste_x_0=teste_x_0+1;}



            if(teste_x_0==1){
                if(lado_tela0!=1){
            tft.fillRoundRect(30,185,40,40,5, CYAN);
            tft.fillRoundRect(250,185,40,40,5, WHITE);
            tft.setCursor(42,200);
            tft.println("<");
            lado_tela0=1;}}

            if(teste_x_0==2){
                if(lado_tela0!=2){
            tft.fillRoundRect(250,185,40,40,5, CYAN);
            tft.fillRoundRect(30,185,40,40,5, WHITE);
            tft.setCursor(42,200);
            tft.println("<");
            lado_tela0=2;}}

            if (botao_teste_tela0==1){tela0=0;fixo_1=1;tela1=1;lado_tela0=0;botao_teste_tela0=0;}
            if (botao_teste_tela0==2){tela0=0;fixo_2=1;tela2=1;lado_tela0=0;botao_teste_tela0=0;} 
            
            }

        //////////////TELA DE OPÇÕES/////////////////////////////////////////////////////////////
            if(tela2==1){
                if(fixo_2==1){sensi();
                tft.fillRoundRect(120, 70,50, 25, 0, WHITE);
                tft.setCursor(130,75);
                tft.println("vol:");
                tft.fillRoundRect(180, 70,35, 25, 0, WHITE);
                tft.setCursor(190,75);
                tft.println(vol);
                tft.fillRoundRect(60, 15,35, 45, 0, WHITE);
                tft.setCursor(65, 30);
                tft.println(contador_x);
                tft.fillRoundRect(160, 15,35, 45, 0, WHITE);
                tft.setCursor(165, 30);
                tft.println(contador_y);
                tft.fillRoundRect(260, 15,35, 45, 0, WHITE);
                tft.setCursor(265,30);
                tft.println(salva_z); 
                tft.setCursor(42,200);
                tft.println("<");
                fixo_2=0;}

                if (y<10 && teste_y_2>0){wait_ms(150);teste_y_2=teste_y_2-1;}  //verifica qual botao 
                if(y>999 && teste_y_2<2){wait_ms(150);teste_y_2=teste_y_2+1;}
                if (x<50 && teste_x_2>0){wait_ms(150);teste_x_2=teste_x_2-1;}
                if(x>950 && teste_x_2<2){wait_ms(150);teste_x_2=teste_x_2+1;}

                if (teste_y_2==1 & teste_x_2==1){
                    if(lado_tela2!=1){
                    tft.fillRoundRect(35, 100, 120, 50, 5,CYAN);
                    tft.fillRoundRect(170, 100, 120, 50, 5,WHITE);
                    tft.fillRoundRect(103, 165, 120, 50, 5,WHITE);
                    tft.fillRoundRect(30,185,40,40,5,WHITE);
                    tft.setCursor(70,115);
                    tft.println("MAPA");
                    tft.setCursor(195,115);
                    tft.println("MANUAL");
                    tft.setCursor(120,180);
                    tft.println("DEFINIR");
                    lado_tela2=1;
                    tft.setCursor(42,200);
                    tft.println("<");
                    wait_ms(300);}}

                    
                if (teste_y_2==1 & teste_x_2==2){
                    if(lado_tela2!=2){
                    tft.fillRoundRect(35, 100, 120, 50, 5,WHITE);
                    tft.fillRoundRect(170, 100, 120, 50, 5,CYAN);
                    tft.fillRoundRect(103, 165, 120, 50, 5,WHITE);
                    tft.fillRoundRect(30,185,40,40,5, WHITE);
                    tft.setCursor(70,115);
                    tft.println("MAPA");
                    tft.setCursor(195,115);
                    tft.println("MANUAL");
                    tft.setCursor(120,180);
                    tft.println("DEFINIR");
                    tft.setCursor(42,200);
                    tft.println("<");
                    lado_tela2=2;
                    wait_ms(300);}}
        
                if (teste_y_2==2 & teste_x_2==2){
                    if(lado_tela2!=3){
                    tft.fillRoundRect(35, 100, 120, 50, 5,WHITE);
                    tft.fillRoundRect(170, 100, 120, 50, 5,WHITE);
                    tft.fillRoundRect(103, 165, 120, 50, 5,CYAN);
                    tft.fillRoundRect(30,185,40,40,5, WHITE);
                    tft.setCursor(70,115);
                    tft.println("MAPA");
                    tft.setCursor(195,115);
                    tft.println("MANUAL");
                    tft.setCursor(120,180);
                    tft.println("DEFINIR");
                    tft.setCursor(42,200);
                    tft.println("<");
                    lado_tela2=3;
                    wait_ms(300);}}

                if (teste_y_2==2 & teste_x_2==1){
                    if(lado_tela2!=4){
                    tft.fillRoundRect(35, 100, 120, 50, 5,WHITE);
                    tft.fillRoundRect(170, 100, 120, 50, 5,WHITE);
                    tft.fillRoundRect(103, 165, 120, 50, 5,WHITE);
                    tft.fillRoundRect(30,185,40,40,5, CYAN);
                    tft.setCursor(70,115);
                    tft.println("MAPA");
                    tft.setCursor(195,115);
                    tft.println("MANUAL");
                    tft.setCursor(120,180);
                    tft.println("DEFINIR");
                    tft.setCursor(42,200);
                    tft.println("<");
                    lado_tela2=4;
                    wait_ms(300);}}
                
                
                if(botao_teste_tela2==2){tela2=0;fixo_3=1;tela3=1; lado_tela2=0; botao_teste_tela2=0;}
                if(botao_teste_tela2==1){tela2=0;fixo_4=1;tela4=1;botao_teste_tela2=0;}
                if(botao_teste_tela2==4){tela2=0;fixo_0=1;tela0=1;lado_tela2=0;botao_teste_tela2=0;}
                if(botao_teste_tela2==3){tela2=0;fixo_5=1;tela5=1;lado_tela2=0;botao_teste_tela2=0;}
            }

        /////////////////////TELA DO JOYSTICK//////////////////////////////////////////////////////////////
            if (tela3==1){
                tft.setTextColor(BLACK);
                if (fixo_3==1){manual(); fixo_3=0;libera=0;}
                if (y<10 && teste_y>0 & libera==0){wait_ms(150);teste_y=teste_y-1;}  //verifica qual botao 
                if(y>950 && teste_y<3 & libera==0){wait_ms(150);teste_y=teste_y+1;}

                //if (x<10 && teste_x_3>0 & libera==0){wait_ms(150);teste_x_3=teste_x_3-1;}
                //if(x>999 && teste_x_3<2 & libera==0){wait_ms(150);teste_x_3=teste_x_3+1;}

                if (teste_y==1){ 
                    if(lado_tela3!=1){
                    tft.setTextColor(BLACK);
                    tft.fillRoundRect(30,185,40,40,5, WHITE);
                    tft.fillRoundRect(30,100,60,60,5, CYAN);
                    tft.setCursor(50,130);
                    tft.println("J");
                    tft.setCursor(42,200);
                    tft.println("<");
                    lado_tela3=1;}}
                if (teste_y==2){ 
                    if(lado_tela3!=2){
                    tft.setTextColor(BLACK);
                    tft.fillRoundRect(30,185,40,40,5, CYAN);
                    tft.fillRoundRect(30,100,60,60,5, WHITE);
                    tft.setCursor(50,130);
                    tft.println("J");
                    tft.setCursor(42,200);
                    tft.println("<");
                    lado_tela3=2;}}


                if(botao_teste_tela3==1){tela3=0;fixo_2=1;tela2=1;lado_tela3=0;teste_x_2=1;teste_y_2=1;botao_teste_tela3=0;}
                if (libera==1){ 
                    jogManual();}
                    
                    
                    
            }
    ////////////////////TELA DO MAPA //////////////////////////////////////////////////////////////////////////////////////
                if (tela4==1){
                    if(fixo_4==1){mapa();fixo_4=0;}
                    if(botao_teste_tela4==1){tela4=0;fixo_2=1;tela2=1;lado_tela2=0;botao_teste_tela4=0;}}

    //// TELA DE HISTÓRICO //////////////////////////////////////////////////////////////////////////////////////////////////
                if (tela5==1){
                    if(fixo_5==1){historico();fixo_5=0;
                    lista_x[index]=contador_x;
                    lista_y[index]=contador_y;
                    lista_z[index]=salva_z;
                    lista_vol[index]=vol;
                    
                    tft.setCursor(11,43);tft.println(lista_x[0]);tft.setCursor(55,43);tft.println(lista_y[0]);tft.setCursor(99,43);tft.println(lista_z[0]);tft.setCursor(140,43);tft.println('-');
                    tft.setCursor(11,73);tft.println(lista_x[1]);tft.setCursor(55,73);tft.println(lista_y[1]);tft.setCursor(99,73);tft.println(lista_z[1]);tft.setCursor(140,73);tft.println(lista_vol[1]);
                    tft.setCursor(11,103);tft.println(lista_x[2]);tft.setCursor(55,103);tft.println(lista_y[2]);tft.setCursor(99,103);tft.println(lista_z[2]);tft.setCursor(140,103);tft.println(lista_vol[2]);
                    tft.setCursor(11,133);tft.println(lista_x[3]);tft.setCursor(55,133);tft.println(lista_y[3]);tft.setCursor(99,133);tft.println(lista_z[3]);tft.setCursor(140,133);tft.println(lista_vol[3]);
                    tft.setCursor(11,166);tft.println(lista_x[4]);tft.setCursor(55,166);tft.println(lista_y[4]);tft.setCursor(99,166);tft.println(lista_z[4]);tft.setCursor(140,166);tft.println(lista_vol[4]);
                    tft.setCursor(170,43);tft.println(lista_x[5]);tft.setCursor(210,43);tft.println(lista_y[5]);tft.setCursor(250,43);tft.println(lista_z[5]);tft.setCursor(290,43);tft.println(lista_vol[5]);
                    tft.setCursor(170,73);tft.println(lista_x[6]);tft.setCursor(210,73);tft.println(lista_y[6]);tft.setCursor(250,73);tft.println(lista_z[6]);tft.setCursor(290,73);tft.println(lista_vol[6]);
                    tft.setCursor(170,103);tft.println(lista_x[7]);tft.setCursor(210,103);tft.println(lista_y[7]);tft.setCursor(250,103);tft.println(lista_z[7]);tft.setCursor(290,103);tft.println(lista_vol[7]);
                    tft.setCursor(170,133);tft.println(lista_x[8]);tft.setCursor(210,133);tft.println(lista_y[8]);tft.setCursor(250,133);tft.println(lista_z[8]);tft.setCursor(290,133);tft.println(lista_vol[8]);
                    tft.setCursor(170,166);tft.println(lista_x[9]);tft.setCursor(210,166);tft.println(lista_y[9]);tft.setCursor(250,166);tft.println(lista_z[9]);tft.setCursor(290,166);tft.println(lista_vol[9]);

                    }
                    
                    if (x<50 && teste_x_5>0){wait_ms(150);teste_x_5=teste_x_5-1;}
                    if(x>950 && teste_x_5<3){wait_ms(150);teste_x_5=teste_x_5+1;}

                    if(teste_x_5==1){
                        if(lado_tela5!=1){
                        tft.fillRoundRect(30,190,40,40,5,CYAN);
                        tft.fillRoundRect(110,190,60,40,5,WHITE);
                        tft.fillRoundRect(190,190,100,40,5,WHITE);
                        tft.setCursor(42,200);
                        tft.println("<");
                        tft.setCursor(200,200);
                        tft.println("pipetar");
                        tft.setCursor(120,200);
                        tft.println("nova");
                        lado_tela5=1;}}

                    if(teste_x_5==2){
                        if(lado_tela5!=2){
                        tft.fillRoundRect(30,190,40,40,5,WHITE);
                        tft.fillRoundRect(110,190,60,40,5,CYAN);
                        tft.fillRoundRect(190,190,100,40,5,WHITE);
                        tft.setCursor(42,200);
                        tft.println("<");
                        tft.setCursor(200,200);
                        tft.println("pipetar");
                        tft.setCursor(120,200);
                        tft.println("nova");
                        lado_tela5=2;}}
            
                    if(teste_x_5==3){
                        if(lado_tela5!=3){
                        tft.fillRoundRect(30,190,40,40,5,WHITE);
                        tft.fillRoundRect(110,190,60,40,5,WHITE);
                        tft.fillRoundRect(190,190,100,40,5,CYAN);
                        tft.setCursor(200,200);
                        tft.println("pipetar");
                        tft.setCursor(120,200);
                        tft.println("nova");
                        tft.setCursor(42,200);
                        tft.println("<");
                        lado_tela5=3;}}

                    
                    if(botao_teste_tela5==1){tela5=0;fixo_2=1;tela2=1;teste_y_2=1;botao_teste_tela5=0;}
                    if(botao_teste_tela5==2){tela5=0;fixo_0=1;tela0=1;teste_y_2=1;botao_teste_tela5=0; if(index>-1 & index<10){index=index+1;}}
                    if(botao_teste_tela5==3){teste_pipeta();botao_teste_tela5=0;}
                    
            }}
        if(estadoBE==1){
            if (fixo_emergencia==1){
            emergencia(); fixo_emergencia=0;}
            fixo_1=1; tela1=1; tela0=0; tela2=0;tela3=0; tela4=0; tela5=0;
        }
    

            }
}

void setup(void){
    tft.reset();
    tft.begin();
    tft.setRotation(Orientation);
    //tft.fillScreen(BLACK); 
    principal();
    //emergencia();
    //historico();
    
    }

void loop(){}