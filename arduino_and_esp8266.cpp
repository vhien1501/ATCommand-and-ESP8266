#define LED_PIN 13

#define CMD_SEND_BEGIN "AT+CIPSEND=0"
#define CMD_SEND_END "AT+CIPCLOSE=0"

#define CMD_ACCESS_POINT_ON "AT+CWMODE_CUR=3"

#define CHAR_CR 0x0D
#define CHAR_LF 0x0A

#define EMPTY_STRING ""

#define DELAY_SEED 1000
#define DELAY_SEED_1x (1*DELAY_SEED)
#define DELAY_SEED_2x (2*DELAY_SEED)
#define DELAY_SEED_3x (3*DELAY_SEED)
#define DELAY_SEED_4x (4*DELAY_SEED)
#define DELAY_SEED_5x (5*DELAY_SEED)

bool hasMessage = false;

void executeCommand(const String& command, int delayTime) {
	Serial.println(command);
	delay(delayTime);
}

void procedure(const String& command) {
	hasMessage = command.startsWith("+IPD,");
	if (command.indexOf("TURN_ON") != -1) {
		digitalWrite(LED_PIN, HIGH);
	} else if (command.indexOf("TURN_OFF") != -1) {
		digitalWrite(LED_PIN, LOW);
	}
}

void bufferingMessage(char c) {
	static String bufferData = EMPTY_STRING;

	switch(c) {
		case CHAR_CR:
			break;
		case CHAR_LF: {
			procedure(bufferData);
			bufferData = EMPTY_STRING;
		}
			break;
		default:
			bufferData += c
	}
}

void initEsp() {
	executeCommand("AT+RST", DELAY_SEED_2x);
	executeCommand("AT+CWMODE_CUR=3", DELAY_SEED_3x);
	executeCommand("AT+CWJAP=\"ESP8266\",\"12345678\"", DELAY_SEED_3x);
	executeCommand("AT+CIFSR", DELAY_SEED_1x);
	executeCommand("AT+CIPMUX=1", DELAY_SEED_1x);
	executeCommand("AT+CIPSERVER=1,80", DELAY_SEED_1x)
	executeCommand("AT+CIPSTART=0,\"UDP\",\"192.168.4.22\",4210, 4210, 2");
}

void setup() {
	delay(DELAY_SEED_5x);
	Serial.begin(115200);
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);

	initEsp();
}

void loop() {
	while(Serial.available()) {
		bufferingMessage(Serial.read());
	}
}
