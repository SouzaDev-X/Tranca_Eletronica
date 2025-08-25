#include <Keypad.h>
#include <Servo.h>

// ===================
// Configuração do Keypad
// ===================
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2,3,4,5};
byte colPins[COLS] = {6,7,8,9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ===================
// Configuração do Servo e LEDs
// ===================
Servo servo;
const byte SERVO_PIN = 10;
const int FECHADO = 0;
const int ABERTO = 90;

// LEDs virtuais no Wokwi
const byte LED_VERDE = 11;   // aberto
const byte LED_VERMELHO = 12; // fechado

// ===================
// Constantes do Projeto
// ===================
const char SENHA[] = "5555";  
const byte TAM = 4;            
const byte TENT_MAX = 1;       
const unsigned long BLOQ_MS = 20000; 

// ===================
// Variáveis de Estado
// ===================
char buffer[TAM];          
byte idx = 0;              
byte tentativas = 0;       
bool bloqueado = false;    
unsigned long tBloq = 0;   

// ===================
// Função para abrir a tranca com LEDs
// ===================
void abrirTranca() {
  // Abrir servo
  servo.write(ABERTO);
  Serial.println("ABERTO");
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, LOW);

  delay(10000); // tempo aberto (ajustado para testes)

  // Fechar servo
  servo.write(FECHADO);
  Serial.println("FECHADO");
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, HIGH);
}

// ===================
// Setup inicial
// ===================
void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  servo.write(FECHADO); 
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, HIGH); // começa fechado

  Serial.println("Simulação Tranca Eletrônica Iniciada");
}

// ===================
// Loop principal
// ===================
void loop() {
  // Se bloqueado, verificar tempo
  if(bloqueado){
    if(millis() - tBloq >= BLOQ_MS){
      bloqueado = false;
      tentativas = 0;
      Serial.println("Bloqueio encerrado. Pode digitar novamente.");
    } else {
      // mostrar countdown a cada loop (opcional)
      unsigned long restante = (BLOQ_MS - (millis()-tBloq)) / 1000;
      Serial.print("Bloqueado por ");
      Serial.print(restante);
      Serial.println(" s");
      delay(1000);
      return; // não processa teclas
    }
  }

  char key = keypad.getKey();
  if(key){
    Serial.print("Tecla pressionada: ");
    Serial.println(key);

    // Apagar último dígito
    if(key == '*'){
      if(idx > 0){
        idx--;
        buffer[idx] = '\0';
        Serial.print("Último dígito apagado, buffer atual: ");
        for(byte i=0;i<idx;i++) Serial.print(buffer[i]);
        Serial.println();
      }
    }
    // Confirmar senha
    else if(key == '#'){
      if(idx == TAM){
        bool correta = true;
        for(byte i=0;i<TAM;i++){
          if(buffer[i] != SENHA[i]){
            correta = false;
            break;
          }
        }

        if(correta){
          Serial.println("Senha correta!");
          abrirTranca();
          tentativas = 0; // zera tentativas
        } else {
          tentativas++;
          Serial.print("Senha incorreta! Tentativa ");
          Serial.println(tentativas);
          if(tentativas >= TENT_MAX){
            bloqueado = true;
            tBloq = millis();
            Serial.println("Máximo de tentativas atingido. Bloqueio 20s.");
          }
        }
      } else {
        Serial.println("Senha incompleta!");
      }
      idx = 0; // limpa buffer após #
      for(byte i=0;i<TAM;i++) buffer[i]='\0';
    }
    // Digito normal
    else if(key >= '0' && key <= '9'){
      if(idx < TAM){
        buffer[idx] = key;
        idx++;
      }
    }
  }
}
