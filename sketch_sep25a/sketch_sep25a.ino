// 핀 설정
const int leftTrigPin = 9;   // 왼쪽 초음파 센서 Trig 핀
const int leftEchoPin = 8;   // 왼쪽 초음파 센서 Echo 핀
const int rightTrigPin = 11;  // 오른쪽 초음파 센서 Trig 핀
const int rightEchoPin = 10;  // 오른쪽 초음파 센서 Echo 핀
const int leftLedPin = 4;    // 왼쪽 LED 핀
const int rightLedPin = 6;   // 오른쪽 LED 핀
const int buzzerPin = 2;     // 수동 부저 핀

// 감지 거리 설정 (단위: cm)
const int alertDistance = 40;  // LED만 켜지는 일반 경고 거리
const int dangerDistance = 20;  // 부저가 울리는 위험 경고 거리

// 속도 감지를 위한 변수
long previousLeftCm = 0;
long previousRightCm = 0;
const int speedThreshold = 15; // cm/0.1초 - 이 값보다 빠르게 다가오면 '빠른 접근'으로 판단

void setup() {
  // 핀 모드 설정
  pinMode(leftTrigPin, OUTPUT);
  pinMode(leftEchoPin, INPUT);
  pinMode(rightTrigPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // 왼쪽 거리 측정 및 경고 로직
  long leftCm = getDistance(leftTrigPin, leftEchoPin);
  
  if (leftCm < alertDistance && leftCm > 0) { // 20cm 이내에 물체가 있을 경우
    long leftSpeed = previousLeftCm - leftCm;

    if (leftCm <= dangerDistance && leftSpeed > speedThreshold) {
      // 위험 경고: 8cm 이내로 빠르게 접근할 때 (LED 깜빡이고 부저 3번)
      flashLEDAndBuzzer(leftLedPin, 3, buzzerPin);
    } else {
      // 일반 경고: 20cm 이내, 10cm보다 멀리 있을 때 (LED만 켜기)
      digitalWrite(leftLedPin, HIGH);
    }
  } else {
    // 물체가 20cm 밖으로 나가면 LED 끄기
    digitalWrite(leftLedPin, LOW);
    // 부저가 울리고 있었다면 끄기
    noTone(buzzerPin);
  }

  // 오른쪽 거리 측정 및 경고 로직 (왼쪽과 동일)
  long rightCm = getDistance(rightTrigPin, rightEchoPin);

  if (rightCm < alertDistance && rightCm > 0) {
    long rightSpeed = previousRightCm - rightCm;
    
    if (rightCm <= dangerDistance && rightSpeed > speedThreshold) {
      flashLEDAndBuzzer(rightLedPin, 3, buzzerPin);
    } else {
      digitalWrite(rightLedPin, HIGH);
    }
  } else {
    digitalWrite(rightLedPin, LOW);
    noTone(buzzerPin);
  }

  // 현재 거리값을 이전 거리 변수에 저장 (다음 루프에서 비교하기 위함)
  previousLeftCm = leftCm;
  previousRightCm = rightCm;
  
  delay(100); // 0.1초마다 반복
}

// 초음파 센서 거리 측정 함수
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distanceCm = duration * 0.034 / 2;
  
  return distanceCm;
}

// LED 깜빡이고 부저 울리는 함수
void flashLEDAndBuzzer(int ledPin, int count, int buzzerPin) {
  for (int i = 0; i < count; i++) {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000); 
    delay(200);
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
    delay(200);
  }
}