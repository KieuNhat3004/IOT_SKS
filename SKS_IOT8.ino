#define BLYNK_PRINT Serial

#include <SPI.h>
#include <Ethernet.h>
#include <TimerOne.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <Eeprom24Cxx.h>
static Eeprom24C eeprom(256 ,0x50);

char auth[] = "nf3wVWR3uC2t7u8SvFMehT6VI8dFRvmu";


byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress arduino_ip ( 192,   168,   2,  2);
IPAddress dns_ip     (  8,   8,   8,   8);
IPAddress gateway_ip ( 192,   168,   2,   1);
IPAddress subnet_mask(255, 255, 255,   0);


BlynkTimer timer, timer1;
WidgetLCD lcd1(V1), lcd2(V2), lcd3(V3), 
lcd21(V4), lcd22(V5), lcd23(V6), 
lcd24(V7), lcd25(V8), lcd26(V9),
lcd27(V11), lcd28(V12);
unsigned long thoigian, hientai = 0;
int aa = 0;
bool a1 = false, a2 = false, a3 = false, a5 = false, a21 = false,
a22 = false, a23 = false, a24 = false, a25 = false, a26 = false, a27 = false, flag1 = false, 
flag2 = false, flag3 = false, flag4 = false, flag5 = false, flag6 = false, flag7 = false;
                                 //34: báo đèn rs, 35 trạng thái internet out
uint8_t bton_Y22 = A15, sinalON = A14, bt_RS = A13, OUT_RS = 23, check_States = 22, count_RS = 0,// địa chỉ nút run, signal dừng bs, dừng  st, tín hiệu Đèn ON (Y24)
kcb1 = A10, kcb2 = A9, kcb3 = A8,                                           //địa chỉ nút khẩn cấp
kcb4 = A7, kcb5 = A6, kcb6 = A5, kcb7 = A4, cbnangsuat = A12, kcall = A11,  //cb hàng hóa , dừng kc all
btst1 = A3, btst2 = 5, btst3 = 2,                      //địa chỉ st
btst4 = 3, btst5 = 6, btst6 = 7, btst7 = 8;          //địa chỉ st
int count1 = 0, count4 = 0, timercho = 10000; /// 180000 biến nhớ nút nhấn chạy, dừng bs, dừng st// time cho: tan suat ghi(ms)

int doi_mrun, doi_hrun, doi_mbs, doi_hbs;
int ssDoi_run, ssDoi_bs, tong_tile;
float tong_tile1;

byte hrun, mrun, srun, /// H M S -- Time chạy
hbs, mbs, sbs,            /// H M S -- lỗi BS chung
hst, mst, sst,            /// H M S -- lỗi ST chung
hbs1, mbs1, sbs1,         /// H M S -- lỗi BS chi tiết 7
hbs2, mbs2, sbs2,
hbs3, mbs3, sbs3,
hbs4, mbs4, sbs4,
hbs5, mbs5, sbs5,
hbs6, mbs6, sbs6,
hbs7, mbs7, sbs7;

int adr_hrun = 1, adr_mrun = 2, adr_srun = 3, adr_hbs = 4, adr_mbs = 5, adr_sbs = 6,
adr_hst = 7, adr_mst = 8, adr_sst = 9, adr_hbs1 = 10, adr_mbs1 = 11, adr_sbs1 = 12,
adr_hbs2 = 13, adr_mbs2 = 14, adr_sbs2 = 15, adr_hbs3 = 16, adr_mbs3 = 17, adr_sbs3 = 18,
adr_hbs4 = 19, adr_mbs4 = 20, adr_sbs4 = 21, adr_hbs5 = 22, adr_mbs5 = 23, adr_sbs5 = 24,
adr_hbs6 = 25, adr_mbs6 = 26, adr_sbs6 = 27, adr_hbs7 = 28, adr_mbs7 = 29, adr_sbs7 = 30;

unsigned int adr_cbnangsuat = 31, sodem = 0;

int r_hrun, r_mrun, r_srun, r_hbs, r_mbs, r_sbs,
r_hst, r_mst, r_sst, r_hbs1, r_mbs1, r_sbs1,
r_hbs2, r_mbs2, r_sbs2, r_hbs3, r_mbs3, r_sbs3,
r_hbs4, r_mbs4, r_sbs4, r_hbs5, r_mbs5, r_sbs5,
r_hbs6, r_mbs6, r_sbs6, r_hbs7, r_mbs7, r_sbs7, r_cbnangsuat;

uint8_t vlkcb1 = 0, vlkcb2 = 0, vlkcb3 = 0,                       /// biến nhớ dừng bồn sơn
vlkcb4 = 0, vlkcb5 = 0, vlkcb6 = 0,vlkcb7 = 0, vlall = 0, /// biến nhớ dừng bồn sơn
vlst1 = 0, vlst2 = 0, vlst3 = 0,                          /// biến nhớ dừng ST
vlst4 = 0, vlst5 = 0, vlst6 = 0,vlst7 = 0;                /// biến nhớ dừng ST
float tanso =0;

String timerRun = "", timerDB = "", timerDT = "", 
timerDB1 = "", timerDB2 = "", timerDB3 = "", 
timerDB4 = "", timerDB5 = "", timerDB6 = "", 
timerDB7 = "";

boolean ac = 0;

void myTimerEvent()
{

  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.

  lcd1.print(0,0, "Time Chuyền Chạy");
  lcd1.print(0,1, timerRun);
    
  lcd2.print(0,0, "Time Dừng Bồn");
  lcd2.print(0,1, timerDB);

  lcd3.print(0,0, "Time Dừng Trạm");
  lcd3.print(0,1, timerDT); 

  lcd21.print(0,0, "Time Dừng Bồn 1");
  lcd21.print(0,1, timerDB1);
  lcd22.print(0,0, "Time Dừng Bồn 2");
  lcd22.print(0,1, timerDB2);

  lcd28.print(0,0, "Tỉ lệ dừng SX");
  lcd28.print(0,1, tong_tile1 + String("%"));
}
void myTimerEvent1()
{
  lcd23.print(0,0, "Time Dừng Bồn 3");
  lcd23.print(0,1, timerDB3);
  lcd24.print(0,0, "Time Dừng Bồn 4");
  lcd24.print(0,1, timerDB4);
  lcd25.print(0,0, "Time Dừng Bồn 5");
  lcd25.print(0,1, timerDB5);
  lcd26.print(0,0, "Time Dừng Bồn 6");
  lcd26.print(0,1, timerDB6);
  lcd27.print(0,0, "Time Dừng Bồn 7");
  lcd27.print(0,1, timerDB7);

  Blynk.virtualWrite(V10, sodem);
  Blynk.virtualWrite(V0, tanso);
  
}


void setup() {
  Serial.begin(9600);
//  Blynk.begin(auth);

  Blynk.begin(auth, "blynk-cloud.com", 80, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);
  pinMode(bton_Y22, INPUT); 
  pinMode(sinalON, INPUT);
  
  pinMode(kcb1, INPUT); 
  pinMode(kcb2, INPUT); 
  pinMode(kcb3, INPUT); 
  pinMode(kcb4, INPUT); 
  pinMode(kcb5, INPUT); 
  pinMode(kcb6, INPUT); 
  pinMode(kcb7, INPUT);
  pinMode(kcall, INPUT);

  pinMode(btst1, INPUT); 
  pinMode(btst2, INPUT); 
  pinMode(btst3, INPUT); 
  pinMode(btst4, INPUT); 
  pinMode(btst5, INPUT); 
  pinMode(btst6, INPUT); 
  pinMode(btst7, INPUT);

  pinMode(cbnangsuat, INPUT); 
  pinMode(bt_RS, INPUT);

  pinMode(OUT_RS, OUTPUT);
  pinMode(check_States, OUTPUT);

  Timer1.initialize(1000000); // khởi tạo timer 1 đến 1 giây
  Timer1.attachInterrupt(Blink);
  
  timer.setInterval(1000L, myTimerEvent);
  timer1.setInterval(1010L, myTimerEvent1);
  
  lcd1.clear();
  lcd2.clear();
  lcd3.clear();
  lcd21.clear();
  lcd22.clear();
  lcd23.clear();
  lcd24.clear();
  lcd25.clear();
  lcd26.clear();
  lcd27.clear();
  lcd28.clear();

  hrun = eeprom.read_1_byte(adr_hrun);
  mrun = eeprom.read_1_byte(adr_mrun);
  srun = eeprom.read_1_byte(adr_srun);
  hbs = eeprom.read_1_byte(adr_hbs);
  mbs = eeprom.read_1_byte(adr_mbs);
  sbs = eeprom.read_1_byte(adr_sbs);
  hst = eeprom.read_1_byte(adr_hst);
  mst = eeprom.read_1_byte(adr_mst);
  sst = eeprom.read_1_byte(adr_sst);

  hbs1 = eeprom.read_1_byte(adr_hbs1);
  mbs1 = eeprom.read_1_byte(adr_mbs1);
  sbs1 = eeprom.read_1_byte(adr_sbs1);
  hbs2 = eeprom.read_1_byte(adr_hbs2);
  mbs2 = eeprom.read_1_byte(adr_mbs2);
  sbs2 = eeprom.read_1_byte(adr_sbs2);
  hbs3 = eeprom.read_1_byte(adr_hbs3);
  mbs3 = eeprom.read_1_byte(adr_mbs3);
  sbs3 = eeprom.read_1_byte(adr_sbs3);
  hbs4 = eeprom.read_1_byte(adr_hbs4);
  mbs4 = eeprom.read_1_byte(adr_mbs4);
  sbs4 = eeprom.read_1_byte(adr_sbs4);
  hbs5 = eeprom.read_1_byte(adr_hbs5);
  mbs5 = eeprom.read_1_byte(adr_mbs5);
  sbs5 = eeprom.read_1_byte(adr_sbs5);
  hbs6 = eeprom.read_1_byte(adr_hbs6);
  mbs6 = eeprom.read_1_byte(adr_mbs6);
  sbs6 = eeprom.read_1_byte(adr_sbs6);
  hbs7 = eeprom.read_1_byte(adr_hbs7);
  mbs7 = eeprom.read_1_byte(adr_mbs7);
  sbs7 = eeprom.read_1_byte(adr_sbs7);

  sodem = eeprom.read_2_byte(adr_cbnangsuat);
  delay(500);
}

void loop() {

  if(!Blynk.connected())
  {
    digitalWrite(OUT_RS, 1);
    Blynk.connect();
  }
  else digitalWrite(OUT_RS, 0);
  
  if(digitalRead(bt_RS) == 0){
    count_RS++;
    hrun = 0, mrun = 0, srun = 0, /// H M S -- Time chạy
    hbs = 0, mbs = 0, sbs = 0,            /// H M S -- lỗi BS chung
    hst = 0, mst = 0, sst = 0,            /// H M S -- lỗi ST chung
    hbs1 = 0, mbs1 = 0, sbs1 = 0,         /// H M S -- lỗi BS chi tiết 7
    hbs2 = 0, mbs2 = 0, sbs2 = 0,
    hbs3 = 0, mbs3 = 0, sbs3 = 0,
    hbs4 = 0, mbs4 = 0, sbs4 = 0,
    hbs5 = 0, mbs5 = 0, sbs5 = 0,
    hbs6 = 0, mbs6 = 0, sbs6 = 0,
    hbs7 = 0, mbs7 = 0, sbs7 = 0, sodem = 0;
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd21.clear();
    lcd22.clear();
    lcd23.clear();
    lcd24.clear();
    lcd25.clear();
    lcd26.clear();
    lcd27.clear();
    lcd28.clear();
      if(count_RS >= 2)
      {
        digitalWrite(OUT_RS, 1);
        count_RS = 0;
      }
    }
    else 
    {
      digitalWrite(OUT_RS, 0);
      count_RS = 0;
    }
  
  thoigian = millis();
//  tanso = map((int)analogRead(A2),1023, 0, 0,360)/6.0;  ///hz
  tanso = map((int)analogRead(A2),0, 1023, 0,85);//hz
  if(digitalRead(bton_Y22) == 0){
    count1 = 1;
  }
  else count1 = 0;     

  if(digitalRead(sinalON) == 0){
    count4 = 1;
  }
  else count4 = 0;
///////////////
  if(digitalRead(kcb1) == 0){
    vlkcb1 = 1;
  }
  else vlkcb1 = 0;

  if(digitalRead(kcb2) == 0){
    vlkcb2 = 1;
  }
  else vlkcb2 = 0;

  if(digitalRead(kcb3) == 0){
    vlkcb3 = 1;
  }
  else vlkcb3 = 0;

  if(digitalRead(kcb4) == 0){
    vlkcb4 = 1;
  }
  else vlkcb4 = 0;

  if(digitalRead(kcb5) == 0){
    vlkcb5 = 1;
  }
  else vlkcb5 = 0;

  if(digitalRead(kcb6) == 0){
    vlkcb6 = 1;
  }
  else vlkcb6 = 0;

  if(digitalRead(kcb7) == 0){
    vlkcb7 = 1;
  }
  else vlkcb7 = 0;

  if(digitalRead(kcall) == 0){             //X25
    vlall = 1;                             //X25
  }
  else vlall = 0;

  ///////////////  đọc nút nhấn ST
  if(digitalRead(btst1) == 0){
    vlst1 = 1;
  }
  else vlst1 = 0;

  if(digitalRead(btst2) == 0){
    vlst2 = 1;
  }
  else vlst2 = 0;

  if(digitalRead(btst3) == 0){
    vlst3 = 1;
  }
  else vlst3 = 0;

  if(digitalRead(btst4) == 0){
    vlst4 = 1;
  }
  else vlst4 = 0;

  if(digitalRead(btst5) == 0){
    vlst5 = 1;
  }
  else vlst5 = 0;

  if(digitalRead(btst6) == 0){
    vlst6 = 1;
  }
  else vlst6 = 0;

  if(digitalRead(btst7) == 0){
    vlst7 = 1;
  }
  else vlst7 = 0;
  
///**********

  Blynk.run();
  timer.run();
  timer1.run();

  timerRun = hrun + String(": ") + mrun + String(": ") + srun; 
  timerDB = hbs + String(": ") + mbs + String(": ") + sbs;
  timerDT = hst + String(": ") + mst + String(": ") + sst;

  timerDB1 = hbs1 + String(": ") + mbs1 + String(": ") + sbs1;
  timerDB2 = hbs2 + String(": ") + mbs2 + String(": ") + sbs2;
  timerDB3 = hbs3 + String(": ") + mbs3 + String(": ") + sbs3;
  timerDB4 = hbs4 + String(": ") + mbs4 + String(": ") + sbs4;
  timerDB5 = hbs5 + String(": ") + mbs5 + String(": ") + sbs5;
  timerDB6 = hbs6 + String(": ") + mbs6 + String(": ") + sbs6;
  timerDB7 = hbs7 + String(": ") + mbs7 + String(": ") + sbs7;
  
  ///////////*****

  if(srun == 0  && a1 == false){
    lcd1.clear();
    a1 = true;
  }
  if(srun >50 &&srun <59){
    a1 = false;
  }

  if(sbs == 0 && a2 == false){
    lcd2.clear();
    a2 = true;
  }
  if(sbs >50 &&sbs <59){
    a2 = false;
  }

  if(sst == 0 && a3 == false){
    lcd3.clear();
    a3 = true;
  }
  if(sst >50 &&sst <59){
    a3 = false;
  }
  //****
  if(sbs1 == 0 && a21 == false){
    lcd21.clear();
    a21 = true;
  }
  if(sbs1 >50 &&sbs1 <59){
    a21 = false;
  }
  if(sbs2 == 0 && a22 == false){
    lcd22.clear();
    a22 = true;
  }
  if(sbs2 >50 &&sbs2 <59){
    a22 = false;
  }
  if(sbs3 == 0 && a23 == false){
    lcd23.clear();
    a23 = true;
  }
  if(sbs3 >50 &&sbs3 <59){
    a23 = false;
  }
  if(sbs4 == 0 && a24 == false){
    lcd24.clear();
    a24 = true;
  }
  if(sbs4 >50 &&sbs4 <59){
    a24 = false;
  }
  if(sbs5 == 0 && a25 == false){
    lcd25.clear();
    a25 = true;
  }
  if(sbs5 >50 &&sbs5 <59){
    a25 = false;
  }
  if(sbs6 == 0 && a26 == false){
    lcd26.clear();
    a26 = true;
  }
  if(sbs6 >50 &&sbs6 <59){
    a26 = false;
  }

  if(sbs7 == 0 && a27 == false){
    lcd27.clear();
    a27 = true;
  }
  if(sbs7 >50 &&sbs7 <59){
    a27 = false;
  }

  //*******
  if(digitalRead(cbnangsuat) == 0 && a5 == false)
  {
    a5 = true;
  }
  if(digitalRead(cbnangsuat) == 1 && a5 == true)
  {
    sodem++;
    a5 = false;
  }
  
  if(thoigian - hientai > timercho)
  {
    hientai = millis();
    eeprom.write_1_byte(adr_hrun, hrun);
    eeprom.write_1_byte(adr_mrun, mrun);
    eeprom.write_1_byte(adr_srun, srun);
    eeprom.write_1_byte(adr_hbs, hbs);
    eeprom.write_1_byte(adr_mbs, mbs);
    eeprom.write_1_byte(adr_sbs, sbs);
    eeprom.write_1_byte(adr_hst, hst);
    eeprom.write_1_byte(adr_mst, mst);
    eeprom.write_1_byte(adr_sst, sst);
  
    eeprom.write_1_byte(adr_hbs1, hbs1);
    eeprom.write_1_byte(adr_mbs1, mbs1);
    eeprom.write_1_byte(adr_sbs1, sbs1);
    eeprom.write_1_byte(adr_hbs2, hbs2);
    eeprom.write_1_byte(adr_mbs2, mbs2);
    eeprom.write_1_byte(adr_sbs2, sbs2);
    eeprom.write_1_byte(adr_hbs3, hbs3);
    eeprom.write_1_byte(adr_mbs3, mbs3);
    eeprom.write_1_byte(adr_sbs3, sbs3);
    eeprom.write_1_byte(adr_hbs4, hbs4);
    eeprom.write_1_byte(adr_mbs4, mbs4);
    eeprom.write_1_byte(adr_sbs4, sbs4);
    eeprom.write_1_byte(adr_hbs5, hbs5);
    eeprom.write_1_byte(adr_mbs5, mbs5);
    eeprom.write_1_byte(adr_sbs5, sbs5);
    eeprom.write_1_byte(adr_hbs6, hbs6);
    eeprom.write_1_byte(adr_mbs6, mbs6);
    eeprom.write_1_byte(adr_sbs6, sbs6);
    eeprom.write_1_byte(adr_hbs7, hbs7);
    eeprom.write_1_byte(adr_mbs7, mbs7);
    eeprom.write_1_byte(adr_sbs7, sbs7);
    eeprom.write_2_byte(adr_cbnangsuat, sodem);
    
    delay(20);
  }

//}
  ////////////////////////////////////******************
  notify_ts();

  doi_mrun = mrun * 60;
  doi_hrun = hrun * 3600;
  doi_mbs = mbs * 60;
  doi_hbs = hbs * 3600;
  ssDoi_run = doi_hrun +  doi_mrun + srun;
  ssDoi_bs = doi_hbs +  doi_mbs + sbs;
  tong_tile = (ssDoi_run + ssDoi_bs);
  tong_tile1 = ((float)ssDoi_bs/(float)tong_tile)*100;
}
void notify_ts()
{
  if(vlst1 == 1)
  {
    Blynk.notify("Trạm dừng 1 lỗi!");
  }

  if(vlst2 == 1)
  {
    Blynk.notify("Trạm dừng 2 lỗi!");
  }
  if(vlst3 == 1)
  {
    Blynk.notify("Trạm dừng 3 lỗi!");
  }

  if(vlst4 == 1)
  {
    Blynk.notify("Trạm dừng 4 lỗi!");
  }
  if(vlst5 == 1)
  {
    Blynk.notify("Trạm dừng 5 lỗi!");
  }

  if(vlst6 == 1)
  {
    Blynk.notify("Trạm dừng 6 lỗi!");
  }
  if(vlst1 == 7)
  {
    Blynk.notify("Trạm dừng 7 lỗi!");
  }
}
void Blink()
{
  
  ac =! ac;
  digitalWrite(check_States, ac);
  if(count1 == 1){  ////chạy running
    srun++;
    if(srun > 59){
      mrun++;
      srun = 0;
      
      if(mrun > 59){
       hrun++;
        mrun = 0;
        if(hrun > 23) hrun = 0;
      }
    }
    
  }

  ////////// tính thời gian dừng bồn sơn ALL

  if(vlall == 1 && count4 == 1){  /// dừng bồn sơn
    sbs++;
    if(sbs > 59){
      mbs++;
      sbs = 0;
      
      if(mbs > 59){
       hbs++;
        mbs = 0;
        if(hbs > 23) hbs = 0;
      }
    }
  }

  ////////// tính thời gian dừng st

  if(vlst1 == 1 || vlst2 == 1 || vlst3 == 1 ||
  vlst4 == 1 || vlst5 == 1 || vlst6 == 1 || vlst7 == 1){  /// dừng st
    sst++;
    if(sst > 59){
      
      sst = 0;
      mst++;
      if(mst > 59){
       
        mst = 0;
        hst++;
        if(hst > 23) hst = 0;
      }
    }
  }

  ////////// tính thời gian dừng bồn sơn chi tiết
  
  //// dừng khẩn cấp bồn 1
  if(vlkcb1 == 1 && count4 == 1){  /// dừng bồn sơn
    sbs1++;
    if(sbs1 > 59){
      mbs1++;
      sbs1 = 0;
      if(mbs1 > 59){
       hbs1++;
        mbs1 = 0;
        if(hbs1 > 23) hbs1 = 0;
      }
    }
  }
  //*****

  //// dừng khẩn cấp bồn 2
  if(vlkcb2 == 1 && count4 == 1){  /// dừng bồn sơn
    sbs2++;
    if(sbs2 > 59){
      mbs2++;
      sbs2 = 0;
      if(mbs2 > 59){
       hbs2++;
        mbs2 = 0;
        if(hbs2 > 23) hbs2 = 0;
      }
    }
  }
  //*****

  //// dừng khẩn cấp bồn 3
  if(vlkcb3 == 1 && count4 == 1){  /// dừng bồn sơn
    sbs3++;
    if(sbs3 > 59){
      mbs3++;
      sbs3 = 0;
      if(mbs3 > 59){
       hbs3++;
        mbs3 = 0;
        if(hbs3 > 23) hbs3 = 0;
      }
    }
  }
  //*****

  //// dừng khẩn cấp bồn 4
  if(vlkcb4 == 1 && count4 == 1){  /// dừng bồn sơn
    sbs4++;
    if(sbs4 > 59){
      mbs4++;
      sbs4 = 0;
      if(mbs4 > 59){
       hbs4++;
        mbs4 = 0;
        if(hbs4 > 23) hbs4 = 0;
      }
    }
  }
  //*****

  //// dừng khẩn cấp bồn 5
  if(vlkcb5 == 1 && count4 == 1){  /// dừng bồn sơn
    sbs5++;
    if(sbs5 > 59){
      mbs5++;
      sbs5 = 0;
      if(mbs5 > 59){
       hbs5++;
        mbs5 = 0;
        if(hbs5 > 23) hbs5 = 0;
      }
    }
  }
  //*****

  //// dừng khẩn cấp bồn 6
  if(vlkcb6 == 1 && count4 == 1){  /// dừng bồn sơn
    sbs6++;
    if(sbs6 > 59){
      mbs6++;
      sbs6 = 0;
      if(mbs6 > 59){
       hbs6++;
        mbs6 = 0;
        if(hbs6 > 23) hbs6 = 0;
      }
    }
  }
  //*****

  //// dừng khẩn cấp bồn 7
  if(vlkcb7 == 1 && count4 == 1){  /// dừng bồn sơn
    sbs7++;
    if(sbs7 > 59){
      mbs7++;
      sbs7 = 0;
      if(mbs7 > 59){     
       hbs7++;
        mbs7 = 0;
        if(hbs7 > 23) hbs7 = 0;
      }
    }
  }
  //*****
} 
