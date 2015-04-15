int DATA_MASK = 0b111000;
int data = 0;
int dataold = 0;

void setup() {
  Serial.begin(115200);
  for(int i=2;i<6;++i)pinMode(i,INPUT);
}

void loop() {
  data = PINE & DATA_MASK;
  data >>= 3;
  if(data && data != dataold) {
    Serial.println(data);    
  }
  dataold = data;
}
