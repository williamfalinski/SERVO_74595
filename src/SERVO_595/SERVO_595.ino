/* 1- INICIALIZACAO DOS SERVOS ........*/
/* 2- SERVO PULSE .....................*/
/* 3- LER DUTY CYCLE DE CADA SERVO ....*/
/* 4- FAZER SERVO PULSE ...............*/
/* 5- ENVIO DE DADOS para o 74595....OK*/

#include <TimerOne.h> //Bilbioteca para utilizar o TIMER1 do Arduino 328p, timer com 16 bits de resolucao
/*Valores especificos para o SG90 XING LING*/
#define MAX_DUTY_CYCLE 2450  //us
#define MIN_DUTY_CYCLE 550   //us
#define PWM_PERIOD     20000 //us
/*******************************************/

#define N_SERVOS  8 //Quantidade de Servos 
#define SER       10//Pino Dados
#define SRCLK     12//Pino Clock de comunicacao
#define RCLK      11//Pino Latch
//#define OE      11//Pino Enable saidas -> GND

uint16_t DUTY_CYCLES[8] = {0,0,0,0,0,0,0,0}; //Periodo em HIGH de cada servo 
uint8_t  DATA_SERVOS[8] = {0,1,0,0,0,1,0,0}; //Vetor de Binários para enviar ao 74595

void setup(){
  pinMode(SER, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  //pinMode(POT, INPUT);
  initialize_servos();
  Timer1.initialize(PWM_PERIOD); // Inicializa o Timer1 e configura para um período de 20 milisegundos
  Timer1.attachInterrupt(servoPulse); // Configura a função callback() como a função para ser chamada a cada interrupção do Timer1
}

/*INICIALIZA SERVOS EM 0º*/
/*Especificar numero de servos para evitar calcular servos inexistentes*/
void initialize_servos(){
  for(int i = 0; i<N_SERVOS; i++){
    DUTY_CYCLES[i] = MIN_DUTY_CYCLE;
  }
}

void servoPulse(){
  uint64_t PWM_Start = micros();
  uint64_t PWM_Now = micros() - PWM_Start;
  /*Enquanto nao completar um pulso*/
  while(PWM_Now < PWM_PERIOD){
    setBits(PWM_Now);
    loadData_74595();
    PWM_Now = micros() - PWM_Start;
  }
}

/*Vetor de Bits para enviar ao 74595*/
void setBits(uint64_t pwm_now){
  for(int i = 0; i < N_SERVOS; i++){
    if(pwm_now < DUTY_CYCLES[i]){
       DATA_SERVOS[i] = 1;
    }
    else{
       DATA_SERVOS[i] = 0;
    }
  }
}

/*GRAVA INFORMAÇÕES NO 74595*/
void loadData_74595(){
  /*CLOCK DO LATCH*/
  digitalWrite(RCLK, LOW);
  /*GRAVA DO ULTIMO PARA O PRIMEIRO BIT*/
  for(int i=N_SERVOS-1; i>=0; i--){
    /*CLOCK DOS REGISTRADORES PRIMARIOS*/
    digitalWrite(SRCLK, LOW);
    digitalWrite(SER, DATA_SERVOS[i]);
    digitalWrite(SRCLK, HIGH);
    /*FIM CLOCK DOS REGISTRADORES PRIMARIOS*/   
  } 
  digitalWrite(RCLK, HIGH); 
  /*FIM CLOCK DO LATCH*/
}


void loop()
{
  //delay(500);
}
