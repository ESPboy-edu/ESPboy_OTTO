int trigPin = 9; 
int echoPin = 8; 
 
void setup() { 
  Serial.begin (9600); // подключаем монитор порта
  pinMode(trigPin, OUTPUT); // назначаем trigPin (Pin8), как выход
  pinMode(echoPin, INPUT); // назначаем echoPin (Pin9), как вход
} 
 
void loop() { 
  unsigned int duration, cm; // назначаем переменную "cm" и "duration" для показаний датчика
  digitalWrite(trigPin, LOW); // изначально датчик не посылает сигнал
  delayMicroseconds(2); // ставим задержку в 2 ммикросекунд

  digitalWrite(trigPin, HIGH); // посылаем сигнал
  delayMicroseconds(10); // ставим задержку в 10 микросекунд
  digitalWrite(trigPin, LOW); // выключаем сигнал

  duration = pulseIn(echoPin, HIGH); // включаем прием сигнала

  cm = duration / 58; // вычисляем расстояние в сантиметрах

  Serial.print(cm); // выводим расстояние в сантиметрах
  Serial.println(" cm");

  delay(100); // ставим паузу в 1 секунду
}
