#include "mbed.h"
#include "Arduino.h"
#include <MCUFRIEND_kbv.h>
#include <TouchScreen_kbv_mbed.h>
#include <cstdio> 
#include "pipe.h"
#define velo 0.0015
Serial pc(USBTX, USBRX); // declara o objeto pc para comunicação serial nativo

InterruptIn botao1(PB_13, PullUp);
AnalogIn xAxis(PC_5);
AnalogIn yAxis(PC_4);
DigitalOut Enable(PB_1); //liga o motor
DigitalOut Enable_x(PB_2); //liga o motor
DigitalOut Sentido(PB_15);
DigitalOut Sentido_x(PB_12);
DigitalOut Passos(PB_14); //pode sern utilizado
DigitalOut rele1(PC_2);
InterruptIn estadoemergencia(PC_8);
DigitalOut Sentido_z(PC_12);
DigitalOut Enable_z(PA_13);
DigitalIn FDC_A_X(PD_2);
DigitalIn FDC_A_Y(PB_7);
DigitalIn FDC_A_X_MENOS(PC_9);
DigitalIn FDC_A_Y_MENOS(PC_10);
DigitalIn FDC_A_Z(PC_3);
DigitalIn FDC_A_Z_MENOS(PC_11);

TSPoint_kbv touch;

const PinName X = D8, Y = A3, XM = A2, YM = D9;   
TouchScreen_kbv ts = TouchScreen_kbv(X, Y, XM, YM, 300);

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
#define GREEN 0x07E0

//tela 0 vai ser a quantidade para pipetagem

int valor_inicial_y;
int tela0=0;int tela1=1; int tela2=0;; int tela3=0; int tela4=0; int tela5=0; int tela6=0;//vai verificar em que tela se está
int fixo_0=1;int fixo_1=1; int fixo_2=1; int fixo_3=1; int fixo_4=1; int fixo_5=1; int fixo_pipeta=0;//vai permitir que a tela só passe uma única vez 
int x,y;
int lado_tela0=0;int lado_tela1=0; int lado_tela2=0; int lado_tela3=0; int lado_tela5=0; int lado_tela6=0;//permite verificar o botao que está sendo pressionado
int teste_y=1; int teste_y_2=1; int teste_x_2=1; int teste_x_3=1; int teste_x_0=1; int teste_x_5=1; int teste_y_1=0;
int vol=1; int muda=0; int libera=0; int letras_tela1=0; int libera_2=0;
int salva_x=0; int salva_y=0; int salva_z=0;
int lista_x[10]={0,0,0,0,0,0,0,0,0,0}; int lista_y[10]={0,0,0,0,0,0,0,0,0,0}; int lista_z[10]; int lista_vol[10]={0,0,0,0,0,0,0,0,0,0}; int index=0; int estado=0;
int pos_atual_y; int contador_y=0;int flag_jogManual = 1;int valor_final_y=10000; int sensibilidade=1800; int pos_atual_x; int contador_x=0; int valor_final_x=10000; int valor_inicial_x=0;
int volume_inicial=0; int volume_final=10; int fixo_1_mov=0; int fixo_1_mov_2=0; int contador_z=0; int valor_inicial_z=0; int valor_final_z=10000;
int botao_teste =0; int botao_teste_tela0=0; int botao_teste_tela2=0; int botao_teste_tela3=0; int botao_teste_tela4=0; int botao_teste_tela5=0;int sensibilidade_x=1000; int botao_teste_tela6=0;
int variavel_pipeta=0; int index_pipeta=0; int fixo_emergencia=1;
bool estadoBE = 0;   int dir_x = 0;int dir_y = 0;int dir_z = 0; int contador_x_coleta=0; float tempo=0.0017;int botoes=0;bool fdc_y=0; int controle_da_pipeta = 1;
bool limite_x = 0;
bool limite_y = 0;
bool limite_z=0;
bool ref=0;



void mapa_p3(){

    tft.reset();
    tft.begin();
    tft.setRotation(Orientation);

    int pressao = 0;
    int x_m = 0;
    int y_m = 0;
    int x_d = 0;
    int y_d = 0;
    int coord_x_m = contador_x/36;
    int coord_y_m = contador_y/36;
    int p_total_x = 0;
    int p_total_y = 0;
    int contador_xm = 0;
    int contador_ym = 0;   
    int caminho_xm = 0;
    int caminho_ym = 0;
    int q_passos_x;
    int q_passos_y;
    int eixo_x;
    int eixo_y;
    

    tft.setRotation(Orientation);
    tft.fillScreen(GRAY);

    tft.fillRoundRect(2,2, altura-4,larg-4,0, WHITE);
    tft.fillRoundRect(6,6,altura-12,larg-12,0, GRAY);
    tft.drawRect(20, 20, 200, 200, WHITE);

    for (int f = 1; f < 11; f++){

        for (int i = 1; i < 11; i++) {

            tft.drawRect(20 * i, 20 * f, 20, 20, WHITE);

        }

    } 

    tft.fillRoundRect(270, 170, 30, 30, 0, MAGENTA);

    while(botao_teste_tela4 == 0 && estadoBE == 0){

        pressao = ts.pressure();

        if (pressao > 200) {

            contador_x = 0;

            contador_y = 0;

            touch = ts.getPoint();

            x = touch.y;

            y = touch.x;

            x_d = int(-41.666 + 0.4166666 * x);

            y_d = int(240 - (-25.961538465 + 0.2884615385 * y));

        }

        if (x_d >= 20 & y_d >= 20 & x_d <= 220 & y_d <= 220) {

            tft.fillRect(x_d, y_d, 10, 10, MAGENTA);

            caminho_xm = x_d - coord_x_m;

            caminho_ym = y_d - coord_y_m;

            q_passos_x = lista_x[index];
            q_passos_y = lista_y[index];

            Enable = 1;
            Enable_x = 1;

            eixo_x = 1;
            eixo_y = 1;

            if (caminho_xm < 0 & caminho_ym < 0) {

                p_total_x = int((abs(caminho_xm) * 36));
                p_total_y = int((abs(caminho_ym) * 36));

                while(contador_x <= p_total_x || contador_y <= p_total_y){

                    if(contador_x <= p_total_x) {

                        Passos = !Passos;
                        Enable_x = 0;
                        Sentido_x = 0;
                        contador_x++;
                        q_passos_x--;    

                    } else {

                        Enable_x = 1;
                        eixo_x = 0;

                    }

                    if(contador_y <= p_total_y) {

                        Passos = !Passos;
                        Enable = 0;
                        Sentido = 0;
                        contador_y++;
                        q_passos_y--;
                        
                    } else {

                        Enable = 1;
                        eixo_y = 0;

                    }

                    wait_us((sensibilidade_x * (eixo_x + eixo_y)) + 1);

                    }

                coord_x_m = x_d;
                coord_y_m = y_d;

            }

            if (caminho_xm > 0 & caminho_ym > 0) {

                p_total_x = int((caminho_xm) * 36);
                p_total_y = int((caminho_ym) * 36);

                while(contador_x <= p_total_x || contador_y <= p_total_y){

                    if(contador_x <= p_total_x) {

                        Passos = !Passos;
                        Enable_x = 0;
                        Sentido_x = 1;
                        contador_x++;
                        q_passos_x++;    

                    } else {

                        Enable_x = 1;
                        eixo_x = 0;

                    }

                    if(contador_y <= p_total_y) {

                        Passos = !Passos;
                        Enable = 0;
                        Sentido = 1;
                        contador_y++;
                        q_passos_y++;
                        
                    } else {

                        Enable = 1;
                        eixo_y = 0;

                    }

                    wait_us((sensibilidade_x * (eixo_x + eixo_y)) + 1);

                    }

                coord_x_m = x_d;
                coord_y_m = y_d;

            }

            if (caminho_xm > 0 & caminho_ym < 0) {

                p_total_x = int((caminho_xm) * 36);
                p_total_y = int(abs(caminho_ym) * 36);

                while(contador_x <= p_total_x || contador_y <= p_total_y){

                    if(contador_x <= p_total_x) {

                        Passos = !Passos;
                        Enable_x = 0;
                        Sentido_x = 1;
                        contador_x++;
                        q_passos_x++;    

                    } else {

                        Enable_x = 1;
                        eixo_x = 0;

                    }

                    if(contador_y <= p_total_y) {

                        Passos = !Passos;
                        Enable = 0;
                        Sentido = 0;
                        contador_y++;
                        q_passos_y--;
                        
                    } else {

                        Enable = 1;
                        eixo_y = 0;

                    }

                    wait_us((sensibilidade_x * (eixo_x + eixo_y)) + 1);

                    }


                coord_x_m = x_d;
                coord_y_m = y_d;

            }

            if (caminho_xm < 0 & caminho_ym > 0) {

                p_total_x = int(abs(caminho_xm) * 36);
                p_total_y = int((caminho_ym) * 36);

                while(contador_x <= p_total_x || contador_y <= p_total_y){

                    if(contador_x <= p_total_x) {

                        Passos = !Passos;
                        Enable_x = 0;
                        Sentido_x = 0;
                        contador_x++;
                        q_passos_x--;    

                    } else {

                        Enable_x = 1;
                        eixo_x = 0;

                    }

                    if(contador_y <= p_total_y) {

                        Passos = !Passos;
                        Enable = 0;
                        Sentido = 1;
                        contador_y++;
                        q_passos_y++;

                    } else {

                        Enable = 1;
                        eixo_y = 0;

                    }

                    wait_us((sensibilidade_x * (eixo_x + eixo_y)) + 1);

                    }


                coord_x_m = x_d;
                coord_y_m = y_d;

            }

        }


        lista_x[index] = q_passos_x;
        lista_y[index] = q_passos_y; 

        if (x_d >= 240 & x_d <= 300 & y_d >= 140 & y_d <= 200) {

            botao_teste_tela4 = 1;
            tft.reset();

        }

    }

}

void tela(){
    tft.fillRoundRect(2,2, altura-4,larg-4,0, WHITE);
    tft.fillRoundRect(6,6,altura-12,larg-12,0, GRAY);
    tft.fillRoundRect(15, 13, altura-30, 45,0, WHITE);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(90,28); 
    tft.println("PIPETAGEM");
    tft.drawRGBBitmap(220,16,pipeta,40,40);
    tft.fillRoundRect(larg-150, 80, 150, 40, 15, WHITE); //botao
    tft.fillRoundRect(larg-150, 140, 150, 40, 15, WHITE);
    tft.setCursor(larg-115,90);
    tft.println("Posicao");
    tft.setCursor(larg-140,150);
    tft.println("Referenciar");
    if(ref==0){
        tft.setCursor(10,210);
        tft.println("Referenciamento");
        tft.setTextColor(RED);
        tft.setCursor(200,210);
        tft.println("pendente");}
    else{
        tft.setCursor(10,210);
        tft.println("Referenciamento");
        tft.setTextColor(GREEN);
        tft.setCursor(200,210);
        tft.println("concluido");
    }
}

void mov_tela1(){
    y=yAxis.read()*1000;
    if(y<300){
        if(lado_tela1!=1){
        y=yAxis.read()*1000;
        tft.setTextColor(BLACK);
        tft.fillRoundRect(larg-150, 80, 150, 40, 15, CYAN); //botao
        tft.fillRoundRect(larg-150, 140, 150, 40, 15, WHITE);
        lado_tela1=1;
        tft.setCursor(larg-115,90);
        tft.println("Posicao");
        tft.setCursor(larg-140,150);
        tft.println("Referenciar");
        wait_ms(300);
        }} 

    if(y>800){
        if(lado_tela1!=2){
        tft.setTextColor(BLACK);
        y=yAxis.read()*1000;
        tft.fillRoundRect(larg-150, 80, 150, 40, 15, WHITE); //botao
        tft.fillRoundRect(larg-150, 140, 150, 40, 15, CYAN);
        tft.setCursor(larg-115,90);
        tft.println("Posicao");
        tft.setCursor(larg-140,150);
        tft.println("Referenciar");
        wait_ms(300);
        lado_tela1=2;}
        }
    }


void sensi(){

    tft.begin();
    tft.setRotation(Orientation);
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
    tft.setCursor(190,75);
    tft.println(vol);
    tft.setCursor(220,75);
    tft.println("ml");
    

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
        tft.fillRoundRect(100,100,60,60,5, WHITE);
        tft.fillRoundRect(210,100,60,60,5, WHITE);
        tft.setCursor(230,120);
        tft.println("Jz");
        tft.setCursor(125,120);
        tft.println("J");
        tft.setCursor(42,200);
        tft.println("<");
        tft.setTextColor(WHITE);
        tft.setCursor(70,120);
        tft.println("-X");
        tft.setCursor(165,120);
        tft.println("+X");
        tft.setCursor(120,80);
        tft.println("-Y");
        tft.setCursor(120,170);
        tft.println("+Y");
        tft.setCursor(230,80);
        tft.println("-Z");
        tft.setCursor(230,170);
        tft.println("+Z");
        
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
        tft.setCursor(265,200);
        tft.println(">");

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
        tft.fillRoundRect(10, 39, 145, 25,0, CYAN); 
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
        tft.setCursor(42,200);
        tft.println("<");
        
        }


void emergencia(){

    tft.begin();
    tft.setRotation(Orientation);
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

void tela_pipetagem(){
    if(fixo_pipeta==0 && estadoBE!=1){
        tft.fillRoundRect(2,2, altura-4,larg-4,0, WHITE);
        tft.fillRoundRect(6,6,altura-12,larg-12,0, GRAY);
        tft.fillRoundRect(15, 13, altura-30, 45,0, WHITE);
        tft.setTextSize(2);
        tft.setTextColor(BLACK);
        tft.setCursor(50,28);
        tft.println("PIPETAGEM INICIADA");
        tft.fillRoundRect(40, 180,100,50,15, WHITE);
        tft.fillRoundRect(170, 180,100,50,15, WHITE);
        tft.setCursor(50,200);
        tft.println("Repetir");
        tft.setCursor(195,190);
        tft.println("Novo");
        tft.setCursor(195,210);
        tft.println("Ciclo");
        
        fixo_pipeta=1;}
        if(lista_vol[index_pipeta]>0 && estadoBE!=1){
            tft.setCursor(80,100);
            tft.println("POSICAO:");
            tft.fillRoundRect(170, 90,30,40,0, GRAY);
            tft.setCursor(180,100);
            tft.println(index_pipeta);
            tft.setCursor(80,150);
            tft.println("FALTAM:");
            tft.setCursor(170,150);
            tft.fillRoundRect(165, 140,30,40,0, GRAY);
            tft.println(lista_vol[index_pipeta]-variavel_pipeta);
            tft.setCursor(190,150);
            tft.println("ml");}
        if(lista_vol[index_pipeta]==0 && estadoBE!=1){
            tft.fillRoundRect(15, 13, altura-30, 45,0, WHITE);
            tft.setTextSize(2);
            tft.setTextColor(BLACK);
            tft.setCursor(50,28);
            tft.println("PIPETAGEM FINALIZADA");
                }


}

void jogManual(){
    int valor_maximo_y;
    int valor_maximo_x;

    y=yAxis.read()*1000;
    x=xAxis.read()*1000;
    //if(y<750 && y>300){Enable=1;}
     if(y>750){
        y=yAxis.read()*1000;
        if(FDC_A_Y_MENOS==1 ){
            valor_maximo_y=contador_y;
            Enable=1;}
        else{
        Enable=0;
        Sentido=1;
        //if(y<400 & y>200){sensibilidade=(0.015*y-2)*1000;}
        //if(y<10){sensibilidade=2000;}
        
        contador_y+=1;}
    }
    if(y<100){
        y=yAxis.read()*1000;
        if(FDC_A_Y==1 || contador_y<=valor_inicial_y){
            valor_inicial_y=0;
            contador_y=valor_inicial_y;
            Enable=1;}
        else{
        Enable=0;
        Sentido=0;
        contador_y-=1;

       
        }}
    if(x<200){
        x=xAxis.read()*1000;
        if(FDC_A_X==1 || contador_x==0){
            contador_x=0;
            Enable_x=1;}
        else {
        Enable_x=0;
        Sentido_x=0;

        contador_x-=1;}}
    
    if(x>800){
        x=xAxis.read()*1000;
        if(FDC_A_X_MENOS==1 ){
            Enable_x=1;}
        else{
            Enable_x=0;
            Sentido_x=1;
            contador_x+=1;}}
    //Passos_x=1;
    Passos=1;
    wait_us(sensibilidade_x);
    //Passos_x=0;
    Passos=0;
    wait_us(sensibilidade_x);
   // wait_us(sensibilidade_x);
    //wait_us(sensibilidade);
    //wait_us(sensibilidade);
    //if(x<800 && x>300){Enable_x=1;}
        }



void novo_z(){
    y=yAxis.read()*1000;
    while(y<100 && FDC_A_Z==0){
        y=yAxis.read()*1000;
        Enable_z=1;
        Sentido_z=1;
        Passos=1;
        wait_us(sensibilidade_x);
        Passos=0;
        wait_us(sensibilidade_x);
        contador_z-=1;}
    while(y>900 && FDC_A_Z==0){
        y=yAxis.read()*1000;
        Enable_z=1;
        Sentido_z=0;
        Passos=1;
        wait_us(sensibilidade_x);
        Passos=0;
        wait_us(sensibilidade_x);
        contador_z+=1;

    }}



void zera_z(){
    limite_z=0;
    while(limite_z == 0){
            Enable_z = 1;
            Sentido_z = 1;
            Passos = 1;
            wait_us(sensibilidade_x);
            Passos=0;
            wait_us(sensibilidade_x);
            while (FDC_A_Z == 1) {
                Sentido_z = 0;
                Passos = 1;
                wait_us(sensibilidade_x);
                Passos=0;
                wait_us(sensibilidade_x);
                if (FDC_A_Z == 0) {
                    limite_z = 1;
                    Enable_z = 0;
                    contador_z=0;

                }

            }

        }

}




void toggle_emergencia1(void){
    estadoBE=1;}
void toggle_emergencia2(void){
    estadoBE=0;}


void toggle(void);
void toggle(){
    
    if(debounce.read_ms()>50){
    if (tela3==1 && lado_tela3==2){libera=!libera;}
    if (tela3==1 && lado_tela3==3){libera_2=!libera_2;}
    if(tela1==1 && lado_tela1==1){botao_teste=1;}
    if(tela1==1 && lado_tela1==2){botao_teste=2;}
    if(tela0==1 && lado_tela0==1){botao_teste_tela0=1;}
    if(tela0==1 && lado_tela0==2){botao_teste_tela0=2;}
    if(tela2==1 && lado_tela2==1){botao_teste_tela2=1;}
    if(tela2==1 && lado_tela2==2){botao_teste_tela2=2;}
    if(tela2==1 && lado_tela2==3){botao_teste_tela2=3;}
    if(tela2==1 && lado_tela2==4){botao_teste_tela2=4;}
    if(tela3==1 && lado_tela3==1){botao_teste_tela3=1;}
    if(tela4==1){botao_teste_tela4=1;}
    if(tela5==1 && lado_tela5==1){botao_teste_tela5=1;}
    if(tela5==1 && lado_tela5==2){botao_teste_tela5=2;}
    if(tela5==1 && lado_tela5==3){botao_teste_tela5=3;}
    if(tela6==1 && lado_tela6==1){botao_teste_tela6=1;}
    if(tela6==1 && lado_tela6==2){botao_teste_tela6=2;}
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
        wait_ms(300);}}
    if(y>900){
        if(vol<=volume_inicial){
            vol=volume_inicial;}
        else{
           
            vol=vol-1;
           
            tft.fillRoundRect(140,100,60,60,5, CYAN);tft.setCursor(165,120);tft.println(vol);fixo_0=0;
            wait_ms(300);}}
    }

void pipetar(void){
        rele1=0;
        //rele2=0;
        wait(0.1);
        //rele2=1;
        rele1=1;

    }


void solta(int index){
    while((contador_x!=lista_x[index] || contador_y!=lista_y[index])&& estadoBE!=1){
        if(contador_x-lista_x[index]<0 ){
            Enable_x=0;
            Sentido_x=1;
            contador_x+=1;}
        if(contador_x-lista_x[index]>0){
            Enable_x=0;
            Sentido_x=0;
            contador_x-=1;}
        if(contador_y-lista_y[index]<0){
            Enable=0;
            Sentido=1;
            contador_y+=1;}
        if(contador_y-lista_y[index]>0){
            Enable=0;
            Sentido=0;
            contador_y-=1;}
        if(contador_x==lista_x[index]){
            Enable_x=1;}
        if(contador_y==lista_y[index]){
            Enable=1;}
        Passos=1;
        wait_us(sensibilidade_x);
        Passos=0;
        wait_us(sensibilidade_x);
    }
}

void pega(){
    while((contador_x!=lista_x[0] || contador_y!=lista_y[0]) && estadoBE!=1){
        if(contador_x-lista_x[0]<0){
            Enable_x=0;
            Sentido_x=1;
            contador_x+=1;}
        if(contador_x-lista_x[0]>0){
            Enable_x=0;
            Sentido_x=0;
            contador_x-=1;}
        if(contador_y-lista_y[0]<0){
            Enable=0;
            Sentido=1;
            contador_y+=1;}
        if(contador_y-lista_y[0]>0){
            Enable=0;
            Sentido=0;
            contador_y-=1;}
        if(contador_x==lista_x[0]){
            Enable_x=1;}
        if(contador_y==lista_y[0]){
            Enable=1;}
        Passos=1;
        wait_us(sensibilidade_x);
        Passos=0;
        wait_us(sensibilidade_x);
    }
    



}

    



void referenciamento() {

    while (limite_x == 0 || limite_y == 0) {

        if(limite_x == 0){

            Enable_x = 0;
            Sentido_x = 0;
            Passos = 1;
            wait_us(sensibilidade_x);
            Passos=0;
            wait_us(sensibilidade_x);

            while (FDC_A_X == 1) {

                Sentido_x = 1;
                Passos = 1;
                wait_us(sensibilidade_x);
                Passos=0;
                wait_us(sensibilidade_x);

                if (FDC_A_X == 0) {

                    limite_x = 1;
                    Enable_x = 1;
                    contador_x=0;

                }

            }

        }

        if(limite_y == 0){
        
            Enable = 0;
            Sentido = 0;
            Passos =1;
            wait_us(sensibilidade_x);
            Passos=0;
            wait_us(sensibilidade_x);

            while (FDC_A_Y == 1) {

                Sentido = 1;
                Passos =1;
                wait_us(sensibilidade_x);
                Passos=0;
                wait_us(sensibilidade_x);
                

                if (FDC_A_Y == 0) {

                    limite_y = 1;
                    Enable = 1;
                    contador_y=0;

                }

            }

        }


    }

}

void referenciamento_total(){
    zera_z();
    referenciamento();
} 

void posicao_z_pega(){
    while(contador_z!=lista_z[0]){
        if(contador_z-lista_z[0]>0){
            y=yAxis.read()*1000;
            Enable_z=1;
            Sentido_z=1;
            contador_z-=1;
        }
        if(contador_z-lista_z[0]<0){
            y=yAxis.read()*1000;
            Enable_z=1;
            Sentido_z=0;
            contador_z+=1;

        }
        if(contador_z==lista_z[0]){
            Enable_z=0;}
        Passos=1;
        wait_us(sensibilidade_x);
        Passos=0;
        wait_us(sensibilidade_x);
    }

}

void teste_pipeta(){
    printf("entrou funcao");
    while(index_pipeta <9 && estadoBE!=1){
    printf(index_pipeta);
            if(variavel_pipeta<lista_vol[index_pipeta]){
                zera_z();
                pega();
                posicao_z_pega();
                wait(1);
                pipetar();
                wait(1);
                zera_z();
                solta(index_pipeta);
                pipetar();
                variavel_pipeta+=1;
                tela_pipetagem();}
            else{
                variavel_pipeta=0;
                index_pipeta+=1;
                tela_pipetagem();
            }
        }
    }


void principal(void){
    botao1.fall(&toggle);
    estadoemergencia.fall(&toggle_emergencia1);
    estadoemergencia.rise(&toggle_emergencia2);
    debounce.start();
    while(1){
        rele1=1;
        //rele2=1;
        Enable=1;
        Enable_x=1;
        Enable_z=0;
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
                if (botao_teste==1 &&index>0 && ref==1){tela1=0;fixo_0=1;tela0=1;lado_tela1=0;botao_teste=0;}
                if (botao_teste==1 &&index==0 && ref==1){tela1=0;fixo_2=1;tela2=1;lado_tela1=0;botao_teste=0;}
                if(botao_teste==2){referenciamento_total();ref=1;fixo_1=1;botao_teste=5;}}


        //////////////TELA DE VOLUME//////////////////////////////////////////////////////////////
        if(tela0==1 ){
            if(fixo_0==1){volume();tft.fillRoundRect(140,100,60,60,5, CYAN);tft.setCursor(160,120);tft.println(vol);fixo_0=0;}
            volumes();
            if (x<50 && teste_x_0>0){wait_ms(300);teste_x_0=teste_x_0-1;}
            if(x>950 && teste_x_0<2){wait_ms(300);teste_x_0=teste_x_0+1;}

            if(teste_x_0==1){
                if(lado_tela0!=1){
                    tft.fillRoundRect(30,185,40,40,5, CYAN);
                    tft.fillRoundRect(250,185,40,40,5, WHITE);
                    tft.setCursor(42,200);
                    tft.println("<");
                    tft.setCursor(265,200);
                    tft.println(">");
                    lado_tela0=1;
                }
            }

            if(teste_x_0==2){
                if(lado_tela0!=2){
                    tft.fillRoundRect(250,185,40,40,5, CYAN);
                    tft.fillRoundRect(30,185,40,40,5, WHITE);
                    tft.setCursor(42,200);
                    tft.println("<");
                    tft.setCursor(265,200);
                    tft.println(">");
                    lado_tela0=2;
                }
            }

            if (botao_teste_tela0==1){tela0=0;fixo_1=1;tela1=1;lado_tela0=0;botao_teste_tela0=0;}
            if (botao_teste_tela0==2){tela0=0;fixo_2=1;tela2=1;lado_tela0=0;botao_teste_tela0=0;} 
            
            }

        //////////////TELA DE OPÇÕES/////////////////////////////////////////////////////////////
            if(tela2==1){
                if(fixo_2==1){sensi();
                    tft.fillRoundRect(120, 70,50, 25, 0, WHITE);
                    tft.setCursor(130,75);
                    tft.println("vol:");
                    tft.fillRoundRect(180, 70,60, 25, 0, WHITE);
                    tft.setCursor(190,75);
                    tft.println(vol);
                    tft.setCursor(220,75);
                    tft.println("ml");
                    tft.fillRoundRect(60, 15,35, 45, 0, WHITE);
                    tft.setCursor(65, 30);
                    tft.println(contador_x/100);
                    tft.fillRoundRect(160, 15,35, 45, 0, WHITE);
                    tft.setCursor(165, 30);
                    tft.println(contador_y/100);
                    tft.fillRoundRect(260, 15,35, 45, 0, WHITE);
                    tft.setCursor(265,30);
                    tft.println(contador_z/100); 
                    tft.setCursor(42,200);
                    tft.println("<");
                    fixo_2=0;
                }

                if (y<10 && teste_y_2>0){wait_ms(300);teste_y_2=teste_y_2-1;}  //verifica qual botao 
                if(y>999 && teste_y_2<2){wait_ms(300);teste_y_2=teste_y_2+1;}
                if (x<50 && teste_x_2>0){wait_ms(300);teste_x_2=teste_x_2-1;}
                if(x>950 && teste_x_2<2){wait_ms(300);teste_x_2=teste_x_2+1;}

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
                        wait_ms(300);
                    }
                }

                    
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
                        wait_ms(300);
                    }
                }
        
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
                        wait_ms(300);
                    }
                }

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
                        wait_ms(300);
                    }
                }
                if(botao_teste_tela2==2){tela2=0;fixo_3=1;tela3=1; lado_tela2=0; botao_teste_tela2=0;}
                if(botao_teste_tela2==1){tela2=0;fixo_4=1;tela4=1;botao_teste_tela2=0;}
                if(botao_teste_tela2==4 && index>0){tela2=0;fixo_0=1;tela0=1;lado_tela2=0;botao_teste_tela2=0;}
                if(botao_teste_tela2==4 && index==0){tela2=0;fixo_1=1;tela1=1;lado_tela2=0;botao_teste_tela2=0;}
                if(botao_teste_tela2==3){tela2=0;fixo_5=1;tela5=1;lado_tela2=0;botao_teste_tela2=0;}
            }
        /////////////////////TELA DO JOYSTICK//////////////////////////////////////////////////////////////
            if (tela3==1){
                tft.setTextColor(BLACK);
                if (fixo_3==1){manual(); fixo_3=0;libera=0;}
                if (x<10 && teste_y>0 & libera==0 & libera_2==0){wait_ms(300);teste_y=teste_y-1;}  //verifica qual botao 
                if(x>950 && teste_y<4 & libera==0 & libera_2==0){wait_ms(300);teste_y=teste_y+1;}
                //if (x<10 && teste_x_3>0 & libera==0){wait_ms(150);teste_x_3=teste_x_3-1;}
                //if(x>999 && teste_x_3<2 & libera==0){wait_ms(150);teste_x_3=teste_x_3+1;}
                if (teste_y==2){ 
                    if(lado_tela3!=2){
                        tft.setTextColor(BLACK);
                        tft.fillRoundRect(30,185,40,40,5, WHITE);
                        tft.fillRoundRect(100,100,60,60,5, CYAN);
                        tft.fillRoundRect(210,100,60,60,5, WHITE);
                        tft.setCursor(230,120);
                        tft.println("Jz");
                        tft.setCursor(125,120);
                        tft.println("J");
                        tft.setCursor(42,200);
                        tft.println("<");
                        lado_tela3=2;
                    }
                }
                if (teste_y==1){ //voltar 
                    if(lado_tela3!=1){
                        tft.setTextColor(BLACK);
                        tft.fillRoundRect(30,185,40,40,5, CYAN);
                        tft.fillRoundRect(100,100,60,60,5, WHITE);
                        tft.fillRoundRect(210,100,60,60,5, WHITE);
                        tft.setCursor(230,120);
                        tft.println("Jz");
                        tft.setCursor(125,120);
                        tft.println("J");
                        tft.setCursor(42,200);
                        tft.println("<");
                        lado_tela3=1;
                    }
                }
                if(teste_y==3){ //jogZ
                    if(lado_tela3!=3){
                        tft.setTextColor(BLACK);
                        tft.fillRoundRect(30,185,40,40,5, WHITE);
                        tft.fillRoundRect(100,100,60,60,5, WHITE);
                        tft.fillRoundRect(210,100,60,60,5, CYAN);
                        tft.setCursor(230,120);
                        tft.println("Jz");
                        tft.setCursor(125,120);
                        tft.println("J");
                        tft.setCursor(42,200);
                        tft.println("<");
                        lado_tela3=3;
                    }
                }


                if(botao_teste_tela3==1){tela3=0;fixo_2=1;tela2=1;lado_tela3=0;teste_x_2=1;teste_y_2=1;botao_teste_tela3=0;}
                if (libera==1){ 
                    jogManual();
                }
                    //mov_z();
                if(libera_2==1){
                    novo_z();
                }                    
            }
    ////////////////////TELA DO MAPA //////////////////////////////////////////////////////////////////////////////////////
                if (tela4==1){
                    if(fixo_4==1){mapa_p3();fixo_4=0;}
                    if(botao_teste_tela4==1){tela4 =0;fixo_2=1;tela2=1;lado_tela2=0;botao_teste_tela4=0;}
                }

    //// TELA DE HISTÓRICO //////////////////////////////////////////////////////////////////////////////////////////////////
                if (tela5==1){
                    if(fixo_5==1){historico();fixo_5=0;
                    lista_x[index]=contador_x;
                    lista_y[index]=contador_y;
                    lista_z[index]=contador_z;
                    lista_vol[index]=vol;
                    
                    tft.setCursor(11,43);tft.println(lista_x[0]/100);tft.setCursor(55,43);tft.println(lista_y[0]/100);tft.setCursor(99,43);tft.println(lista_z[0]/100);tft.setCursor(140,43);tft.println('-');
                    tft.setCursor(11,73);tft.println(lista_x[1]/100);tft.setCursor(55,73);tft.println(lista_y[1]/100);tft.setCursor(99,73);tft.println(lista_z[1]/100);tft.setCursor(140,73);tft.println(lista_vol[1]);
                    tft.setCursor(11,103);tft.println(lista_x[2]/100);tft.setCursor(55,103);tft.println(lista_y[2]/100);tft.setCursor(99,103);tft.println(lista_z[2]/100);tft.setCursor(140,103);tft.println(lista_vol[2]);
                    tft.setCursor(11,133);tft.println(lista_x[3]/100);tft.setCursor(55,133);tft.println(lista_y[3]/100);tft.setCursor(99,133);tft.println(lista_z[3]/100);tft.setCursor(140,133);tft.println(lista_vol[3]);
                    tft.setCursor(11,166);tft.println(lista_x[4]/100);tft.setCursor(55,166);tft.println(lista_y[4]/100);tft.setCursor(99,166);tft.println(lista_z[4]/100);tft.setCursor(140,166);tft.println(lista_vol[4]);
                    tft.setCursor(170,43);tft.println(lista_x[5]/100);tft.setCursor(210,43);tft.println(lista_y[5]/100);tft.setCursor(250,43);tft.println(lista_z[5]/100);tft.setCursor(290,43);tft.println(lista_vol[5]);
                    tft.setCursor(170,73);tft.println(lista_x[6]/100);tft.setCursor(210,73);tft.println(lista_y[6]/100);tft.setCursor(250,73);tft.println(lista_z[6]/100);tft.setCursor(290,73);tft.println(lista_vol[6]);
                    tft.setCursor(170,103);tft.println(lista_x[7]/100);tft.setCursor(210,103);tft.println(lista_y[7]/100);tft.setCursor(250,103);tft.println(lista_z[7]/100);tft.setCursor(290,103);tft.println(lista_vol[7]);
                    tft.setCursor(170,133);tft.println(lista_x[8]/100);tft.setCursor(210,133);tft.println(lista_y[8]/100);tft.setCursor(250,133);tft.println(lista_z[8]/100);tft.setCursor(290,133);tft.println(lista_vol[8]);
                    tft.setCursor(170,166);tft.println(lista_x[9]/100);tft.setCursor(210,166);tft.println(lista_y[9]/100);tft.setCursor(250,166);tft.println(lista_z[9]/100);tft.setCursor(290,166);tft.println(lista_vol[9]);

                    }
                    
                    if (x<50 && teste_x_5>0){wait_ms(300);teste_x_5=teste_x_5-1;}
                    if(x>950 && teste_x_5<3){wait_ms(300);teste_x_5=teste_x_5+1;}

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
                            lado_tela5=1;
                        }
                    }

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
                            lado_tela5=2;
                        }
                    }
            
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
                            lado_tela5=3;
                        }
                    }

                    
                    if(botao_teste_tela5==1){tela5=0;fixo_2=1;tela2=1;teste_y_2=1;botao_teste_tela5=0;}
                    if(botao_teste_tela5==2){tela5=0;fixo_0=1;tela0=1;teste_y_2=1;botao_teste_tela5=0; if(index>-1 & index<10){index=index+1;}}
                    if(botao_teste_tela5==3){tela5=0;tela6=1;botao_teste_tela5=0;controle_da_pipeta=1;}
                    
            }
        if (tela6==1){
            

            if (controle_da_pipeta == 1){
                teste_pipeta();
                botao_teste_tela6=0;
            } 

            controle_da_pipeta = 0;

            if(x<10){
                if(lado_tela6!=1){
                tft.fillRoundRect(40, 180,100,50,15, CYAN);
                tft.fillRoundRect(170, 180,100,50,15, WHITE);
                tft.setCursor(50,200);
                tft.println("Repetir");
                tft.setCursor(195,190);
                tft.println("Novo");
                tft.setCursor(195,210);
                tft.println("Ciclo");
                lado_tela6=1;}}
            if(x>950){        
                if(lado_tela6!=2){
                tft.fillRoundRect(40, 180,100,50,15, WHITE);
                tft.fillRoundRect(170, 180,100,50,15, CYAN);
                tft.setCursor(50,200);
                tft.println("Repetir");
                tft.setCursor(195,190);
                tft.println("Novo");
                tft.setCursor(195,210);
                tft.println("Ciclo");
                lado_tela6=2;}}
            if(botao_teste_tela6==2){
                fixo_1=1;
                tela1=1;
                tela0=0; 
                tela2=0;
                tela3=0; 
                tela4=0; 
                tela5=0; 
                tela6=0;
                ref=0;
                limite_x=0; 
                limite_y=0;
                index=0;
                fixo_pipeta=0;
                index_pipeta=0;
                variavel_pipeta=0;
                controle_da_pipeta = 1;
                vol=1;
            for(int a=0; a<10; a++){
                lista_y[a]=0;
                lista_vol[a]=0;
                lista_x[a]=0;
                lista_z[a]=0;
            }}  //fazer um for 
            if(botao_teste_tela6==1){
            tft.fillRoundRect(15, 13, altura-30, 45,0, WHITE);
            tft.setTextSize(2);
            tft.setTextColor(BLACK);
            tft.setCursor(50,28);
            tft.println("PIPETAGEM INICIADA");
            index_pipeta=0;
            variavel_pipeta=0;
            controle_da_pipeta = 1;
            }
        }
        }
        if(estadoBE==1){
            if (fixo_emergencia==1){
                emergencia(); fixo_emergencia=0;}
                fixo_1=1;
                tela1=1;
                tela0=0; 
                tela2=0;
                tela3=0; 
                tela4=0; 
                tela5=0; 
                tela6=0;
                ref=0;
                limite_x=0; 
                limite_y=0;
                index=0;
                fixo_pipeta=0;
                index_pipeta=0;
                variavel_pipeta=0;
                controle_da_pipeta = 1;
                vol=1;
                for(int a=0; a<10; a++){
                    lista_y[a]=0;
                    lista_vol[a]=0;
                    lista_x[a]=0;
                    lista_z[a]=0;
                }} 

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
    //tela_pipetagem();
    //sensi();
    }

void loop(){}