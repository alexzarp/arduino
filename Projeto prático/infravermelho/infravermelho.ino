// INCLUSÃO DE BIBLIOTECA
#include <IRremote.h>
#include <PushButton.h>

// DIRETIVAS DE COMPILAÇÃO
#define tempoTecla 350
#define frequencia 38 // kHz

// DEFINIÇÃO DOS PINOS
#define pinReceptor 6

#define pinEmissor 3
#define pinBot1 8
#define pinLed 2
#define pinLiga 5
#define pinDesliga 4

// INSTANCIANDO OBJETOS
IRrecv receptorIR(pinReceptor);
IRsend emissorIR;
//PushButton botaoLeitura(pinBot1);
PushButton botaoLiga(pinLiga);
PushButton botaoDesliga(pinDesliga);

// DECLARAÇÃO VARIÁVEIS GLOBAIS
bool lerComando = true;

// DECLARAÇÃO DAS FUNÇÕES DE LEITURA

void ircode   (decode_results *results);
void encoding (decode_results *results);
void dumpInfo (decode_results *results);
void dumpRaw  (decode_results *results);
void dumpCode (decode_results *results);

//  DECLARAÇÃO DAS TECLAS CLONADAS

unsigned int desliga[] = {500,1650, 600,1600, 500,1650, 550,1750, 400,1650, 550,1650, 500,1700, 500,1650, 500,700, 450,650, 450,700, 450,650, 450,650, 450,700, 450,650, 400,700, 450,1650, 550,1800, 350,1650, 550,1750, 400,1650, 550,1650, 450,1750, 500,1650, 500,700, 450,650, 450,650, 450,700, 450,650, 400,700, 450,700, 450,650, 450,1650, 550,1650, 500,1650, 550,1800, 350,1650, 550,1650, 500,1650, 550,1650, 500,700, 450,650, 450,650, 450,700, 450,650, 450,650, 450,650, 450,650, 550,1600, 600,1600};
unsigned int liga[] = {250,1900, 250,1950, 250,1900, 200,2000, 250,1900, 250,1950, 250,1900, 300,850, 250,900, 200,900, 200,900, 250,900, 200,900, 250,850, 250,850, 250,1900, 300,1950, 200,1850, 350,1850, 300,1800, 400,1800, 350,1800, 400,1750, 350,600, 600,500, 600,550, 550,550, 600,650, 350,800, 450,700, 300,850, 350,1750, 350,1900, 350,1800, 350,1900, 250,1900, 300,2000, 150,1950, 200,2000, 200,950, 100,1050, 100};
unsigned int teclaC[] = {8900,4500, 500,600, 500,650, 450,650, 450,650, 450,650, 450,650, 500,600, 500,600, 500,1700, 500,600, 500,1750, 500,1700, 500,1700, 500,1700, 500,650, 450,1750, 500,600, 500,600, 500,600, 500,1700, 500,650, 450,650, 500,600, 500,600, 500,1700, 500,1750, 450,1750, 500,600, 500,1750, 450,1750, 450,1750, 500,1700, 500};
unsigned int teclaD[] = {/*COPIE_OS_DADOS_DENTRO_DAS_CHAVES_DO_MONITOR_SERIAL_E_COLE_AQUI*/};

void setup() {
  Serial.begin(9600);
//  pinMode(pinLed, OUTPUT);
  pinMode(pinDesliga, INPUT_PULLUP);
  pinMode(pinLiga, INPUT_PULLUP);

  // INICIANDO RECEPTOR IR
  receptorIR.enableIRIn();
  Serial.print("Setup Concluído");

}

void loop() {
  // MÉTODO PARA LEITURA E ATUALIZAÇÃO DAS PROPRIEDADES DO BOTÃO
  botaoLiga.button_loop();
  botaoDesliga.button_loop();

//  // BLOCO CONDIÇÕES PARA INICIAR LEITURA
//  if (botaoLeitura.pressed() && !lerComando) {
//    lerComando = true;
//    digitalWrite(pinLed, HIGH);
//  } else if (botaoLeitura.pressed() && lerComando) {
//    lerComando = false;
//    digitalWrite(pinLed, LOW);
//  }

  if (botaoDesliga.pressed()) {
    emissorIR.sendRaw(desliga, sizeof(desliga) / sizeof(desliga[0]), frequencia);
    Serial.println("Enviando Tecla desliga clonada");
    delay(tempoTecla);
  }

  if (botaoLiga.pressed()) {
    emissorIR.sendRaw(liga, sizeof(liga) / sizeof(liga[0]), frequencia);
      Serial.println("Enviando Tecla liga clonada");
      delay(tempoTecla);
  }

  // LAÇO PARA LEITURA DO RECEPTOR IR QUANDO FOR PRESSIONADO O BOTÃO
  while (lerComando) {

    decode_results  results;

    if (receptorIR.decode(&results)) {
      ircode   (&results);
      encoding (&results);
      dumpInfo (&results);
      dumpRaw  (&results);
      dumpCode (&results);
      receptorIR.resume();
      lerComando = false;
      digitalWrite(pinLed, LOW);
    }
  }

  // BLOCO PARA RECEBER DADOS DA SERIAL E INICIAR EMISSOR IR
  if (Serial.available()) {
    char tecla = Serial.read();

    switch (tecla) {
//      case 'a':
//        emissorIR.sendRaw(teclaA, sizeof(teclaA) / sizeof(teclaA[0]), frequencia);
//        Serial.println("Enviando Tecla A clonada");
//        delay(tempoTecla);
//        break;
//
//      case 'b':
//        emissorIR.sendRaw(teclaB, sizeof(teclaB) / sizeof(teclaB[0]), frequencia);
//        Serial.println("Enviando Tecla B clonada");
//        delay(tempoTecla);
//        break;

      case 'c':
        emissorIR.sendRaw(teclaC, sizeof(teclaC) / sizeof(teclaC[0]), frequencia);
        Serial.println("Enviando Tecla C clonada");
        delay(tempoTecla);
        break;

      case 'd':
        emissorIR.sendRaw(teclaD, sizeof(teclaD) / sizeof(teclaD[0]), frequencia);
        Serial.println("Enviando Tecla D clonada");
        delay(tempoTecla);
        break;
    }
  }
}
