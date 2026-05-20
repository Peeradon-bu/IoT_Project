# ESP32 Ultrasonic to Supabase & Web Dashboard

ระบบตรวจจับระยะทางด้วยเซนเซอร์ Ultrasonic (HC-SR04) ควบคุมด้วย ESP32 ส่งข้อมูลตรงเข้าฐานข้อมูล Supabase และนำข้อมูลไปแสดงผลบนหน้าเว็บไซต์ในรูปแบบ Dashboard หากมีการเปิด Website จะทำการส่ง API ไปที่ Discord webhook เพื่อแจ้งเตือน

## อุปกรณ์ที่ใช้ (Hardware Requirements)
1. **ESP32** (Development Board)
2. **HC-SR04** (Ultrasonic Sensor)
3. สายไฟ Jumper

### การต่อวงจร (Pin Mapping)
- **Sensor VCC** --> ESP32 **5V** หรือ **3V3** (ขึ้นอยู่กับรุ่นของเซนเซอร์)
- **Sensor GND** --> ESP32 **GND**
- **Sensor Trig** --> ESP32 **GPIO [ใส่เลขขาที่ใช้]**
- **Sensor Echo** --> ESP32 **GPIO [ใส่เลขขาที่ใช้]**

---

## การตั้งค่า Supabase (Database Setup)
1. สร้างโปรเจกต์ใหม่บน [Supabase](https://supabase.com)
2. ไปที่ช่อง **SQL Editor** แล้วรันคำสั่งด้านล่างเพื่อสร้างตารางเก็บข้อมูล:

```sql
CREATE TABLE sensor_data (
    id SERIAL PRIMARY KEY,
    distance FLOAT NOT NULL,
    created_at TIMESTAMPTZ DEFAULT NOW()
);

## การตั้งค่า ESP32
1. ตั้งค่า ssid = "ชื่อ WIFI";
2. ตั้งค่า password = "พาสเวิร์ดของ WIFI";
3. ตั้งค่า supabase_url = "นำ Supabase project url จาก Supabase มาวาง";
4. ตั้งค่า supabase_key = "นำ Supabase key จาก Supabase มาวาง";
5. นำ Script จาก Ultrasonic_Project ไป Upload เข้าบอร์ด ESP32

## การตั้งค่า Javascript & Website
1. ตั้งค่า SUPABASE_URL = 'นำ Supabase project url จาก Supabase มาวาง';
2. ตั้งค่า SUPABASE_KEY = 'นำ Supabase key จาก Supabase มาวาง';
3. ตั้งค่าDISCORD_WEBHOOK_URL = 'นำ Webhook URL จาก Discord มาวาง';
4. นำ Website ทั้งหมด upload ขึ้้นบน Github เพื่อนำไปใช้แบบ public

## วิธีการทำงานของ Hardware & Software
1. ESP32 พยายามเชื่อมต่อ WIFI ที่ตั้งไว้
2. Ultrasonic Sensor ทำการปล่อยคลื่นและรับคลื่นอัตราโซนิกที่สะท้อนกลับมาเพื่อวัดระยะทาง
3. ESP32 board ทำการเก็บค่าและเรียกใช้ฟังก์ชั่น sendToSupabase เพื่อนำ Distance ที่คำนวณส่งช้อมูลไปที่ Supabase
4. JavaScript บนหน้าเว็บทำการดึงข้อมูลล่าสุดจาก Supabase และเรียกใช้ฟังก์ชัน Update เพื่ออัปเดตข้อมูลและรีเฟรชการแสดงผลบนเว็บไซต์
5. นำข้อมูลระยะทางที่ได้มาจัดรูปแบบและแสดงผลบน HTML Dashboard
6. JavaScript ทำการจัดรูปแบบข้อความ Formatting ให้สวยงาม แล้วส่งข้อมูลแจ้งเตือนไปยัง Discord Webhook API