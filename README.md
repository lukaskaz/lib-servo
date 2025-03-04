# lib-servo
Library for controlling servo drives
<br><br>
**Example of the output (w/o and w/ debug logs):**<br>
pi@raspberry:\~/git/lib-servo/build $ ./examples/digital/rpi/pca9685/prog 4 25 0 "/sys/class/pwm/pwmchip0"<br>
First scenario -> moving all servos to given position<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip0/pwm0<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 0/0/240/4166667/0.000000<br>
[INFO][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)] Created servo [id/mount/startpos/endpos]: 0/inverted/19.200000/52.800000<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip0/pwm1<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 1/0/240/4166667/0.000000<br>
[INFO][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)] Created servo [id/mount/startpos/endpos]: 1/normal/19.200000/52.800000<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip0/pwm2<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 2/0/240/4166667/0.000000<br>
[INFO][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)] Created servo [id/mount/startpos/endpos]: 2/inverted/19.200000/52.800000<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip0/pwm3<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 3/0/240/4166667/0.000000<br>
[INFO][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)] Created servo [id/mount/startpos/endpos]: 3/normal/19.200000/52.800000<br>
[INFO][servo::group::Servo::Handler::Handler(const servo::group::config_t&)] Created group of servos num: 4<br>
Moving to servo position: "25.000000"[%]<br>
Press [enter]<br>
![Ex. of controlling four servos, set pos 25%](pictures/four_servos_set_all_pos_25_perc.jpg "Moving four servos to user set position of 25% (1150/1850us)")<br>
Moving to servo position: "NEUTRAL"<br>
Press [enter]<br>
![Ex. of controlling four servos, set pos neutral 0%](pictures/four_servos_set_all_pos_neutral_0_perc.jpg "Moving four servos to neutral position of 0% (1500us)")<br>
Moving to servo position: "RIGHT"<br>
Press [enter]<br>
![Ex. of controlling four servos, set pos right 50%](pictures/four_servos_set_all_pos_right_50_perc.jpg "Moving four servos to right position of 50% (2200/800us)")<br>
Moving to servo position: "LEFT"<br>
Press [enter]<br>
![Ex. of controlling four servos, set pos left -50%](pictures/four_servos_set_all_pos_left_-50_perc.jpg "Moving four servos to left position of -50% (800/2200us)")<br>
First scenario DONE -> disengaging servos<br>
[INFO][servo::group::Servo::Handler::~Handler()] Removed group of servos num: 4<br>
[INFO][servo::rpi::pca9685::Servo::Handler::~Handler()] Removed servo: 0<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 0<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip0/pwm0<br>
[INFO][servo::rpi::pca9685::Servo::Handler::~Handler()] Removed servo: 1<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 1<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip0/pwm1<br>
[INFO][servo::rpi::pca9685::Servo::Handler::~Handler()] Removed servo: 2<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 2<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip0/pwm2<br>
[INFO][servo::rpi::pca9685::Servo::Handler::~Handler()] Removed servo: 3<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 3<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip0/pwm3<br>
<br>
pi@raspberry:\~/git/lib-servo/build $ ./examples/digital/rpi/pca9685/prog 4 25 1 "/sys/class/pwm/pwmchip0"<br>
First scenario -> moving all servos to given position<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[0]: '36.000000' from pulse '1500'<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[0]: '19.200000' from pulse '800'<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[0]: '52.800000' from pulse '2200'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[0]: '4166667' from freq '240'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'export'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip0/pwm0<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: 'normal' to 'polarity'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[0]: '4166667' from freq '240'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '4166667' to 'period'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[0]: '0' from pct '0.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '1' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 0/0/240/4166667/0.000000<br>
[INFO][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)] Created servo [id/mount/startpos/endpos]: 0/inverted/19.200000/52.800000<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[1]: '36.000000' from pulse '1500'<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[1]: '19.200000' from pulse '800'<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[1]: '52.800000' from pulse '2200'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[1]: '4166667' from freq '240'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '1' to 'export'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip0/pwm1<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: 'normal' to 'polarity'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[1]: '4166667' from freq '240'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '4166667' to 'period'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[1]: '0' from pct '0.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '0' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '1' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 1/0/240/4166667/0.000000<br>
[INFO][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)] Created servo [id/mount/startpos/endpos]: 1/normal/19.200000/52.800000<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[2]: '36.000000' from pulse '1500'<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[2]: '19.200000' from pulse '800'<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[2]: '52.800000' from pulse '2200'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[2]: '4166667' from freq '240'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '2' to 'export'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip0/pwm2<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: 'normal' to 'polarity'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[2]: '4166667' from freq '240'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '4166667' to 'period'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[2]: '0' from pct '0.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '0' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '1' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 2/0/240/4166667/0.000000<br>
[INFO][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)] Created servo [id/mount/startpos/endpos]: 2/inverted/19.200000/52.800000<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[3]: '36.000000' from pulse '1500'<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[3]: '19.200000' from pulse '800'<br>
[DBG][double servo::rpi::pca9685::Servo::Handler::posfrompulse(std::chrono::microseconds)] Position[3]: '52.800000' from pulse '2200'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[3]: '4166667' from freq '240'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '3' to 'export'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip0/pwm3<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: 'normal' to 'polarity'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[3]: '4166667' from freq '240'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '4166667' to 'period'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[3]: '0' from pct '0.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '0' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '1' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 3/0/240/4166667/0.000000<br>
[INFO][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)] Created servo [id/mount/startpos/endpos]: 3/normal/19.200000/52.800000<br>
[INFO][servo::group::Servo::Handler::Handler(const servo::group::config_t&)] Created group of servos num: 4<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[0/norm]: '27.600000' from perc '25.000000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[0]: '1150000' from pct '27.600000'<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[1/invr]: '44.400000' from perc '25.000000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[1]: '1850000' from pct '44.400000'<br>
Moving to servo position: "25.000000"[%]<br>
Press [enter]<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[2/norm]: '27.600000' from perc '25.000000'<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[3/invr]: '44.400000' from perc '25.000000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[2]: '1150000' from pct '27.600000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[3]: '1850000' from pct '44.400000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '1850000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '1150000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '1850000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '1150000' to 'duty_cycle'<br>
![Ex. of controlling four servos, set pos 25%](pictures/four_servos_set_all_pos_25_perc.jpg "Moving four servos to user set position of 25% (1150/1850us)")<br>
<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[0/norm]: '36.000000' from perc '0.000000'<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[1/invr]: '36.000000' from perc '0.000000'<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>[] Servo duty[2/norm]: '36.000000' from perc '0.000000'Moving to servo position:<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[0]: '1500000' from pct '36.000000'<br>
"NEUTRAL"[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[2]: '1500000' from pct '36.000000'<br>
<br>
Press [enter][DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[3/invr]: '36.000000' from perc '0.000000'<br>
DBG[][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[1]: '1500000' from pct '36.000000'<br>
DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[3]: '1500000' from pct '36.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '1500000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '1500000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '1500000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '1500000' to 'duty_cycle'<br>
![Ex. of controlling four servos, set pos neutral 0%](pictures/four_servos_set_all_pos_neutral_0_perc.jpg "Moving four servos to neutral position of 0% (1500us)")<br>
<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[0/norm]: '19.200000' from perc '50.000000'<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[1/invr]: '52.800000' from perc '50.000000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[0]: '800000' from pct '19.200000'<br>
Moving to servo position: "RIGHT"<br>
Press [enter][DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[2/norm]: '19.200000' from perc '50.000000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[2]: '800000' from pct '19.200000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[1]: '2200000' from pct '52.800000'<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[3/invr]: '52.800000' from perc '50.000000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[3]: '2200000' from pct '52.800000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '800000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '2200000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '800000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '2200000' to 'duty_cycle'<br>
![Ex. of controlling four servos, set pos right 50%](pictures/four_servos_set_all_pos_right_50_perc.jpg "Moving four servos to right position of 50% (2200/800us)")<br>
<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[0/norm]: '52.800000' from perc '-50.000000'<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[1/invr]: '19.200000' from perc '-50.000000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[0]: '2200000' from pct '52.800000'<br>
[[DBGMoving to servo position: ][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[2/norm]: '52.800000' from perc '-50.000000'<br>
DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[1]: '800000' from pct '19.200000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[2]: '2200000' from pct '52.800000'<br>
[DBG][servo::rpi::pca9685::Servo::Handler::Handler(const servo::rpi::pca9685::config_t&)::<lambda(double)>] Servo duty[3/invr]: '19.200000' from perc '-50.000000'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[3]: '800000' from pct '19.200000'<br>
"LEFT"<br>
Press [enter][DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '800000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '2200000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '2200000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '800000' to 'duty_cycle'<br>
![Ex. of controlling four servos, set pos left -50%](pictures/four_servos_set_all_pos_left_-50_perc.jpg "Moving four servos to left position of -50% (800/2200us)")<br>
<br>
First scenario DONE -> disengaging servos<br>
[INFO][servo::group::Servo::Handler::~Handler()] Removed group of servos num: 4<br>
[INFO][servo::rpi::pca9685::Servo::Handler::~Handler()] Removed servo: 0<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[0]: '0' from pct '0.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'duty_cycle'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[0]: '0' from freq '0'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'period'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 0<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'unexport'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip0/pwm0<br>
[INFO][servo::rpi::pca9685::Servo::Handler::~Handler()] Removed servo: 1<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[1]: '0' from pct '0.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '0' to 'duty_cycle'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[1]: '0' from freq '0'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '0' to 'period'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '0' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 1<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '1' to 'unexport'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip0/pwm1<br>
[INFO][servo::rpi::pca9685::Servo::Handler::~Handler()] Removed servo: 2<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[2]: '0' from pct '0.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '0' to 'duty_cycle'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[2]: '0' from freq '0'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '0' to 'period'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '0' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 2<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[2]: '2' to 'unexport'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip0/pwm2<br>
[INFO][servo::rpi::pca9685::Servo::Handler::~Handler()] Removed servo: 3<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[3]: '0' from pct '0.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '0' to 'duty_cycle'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[3]: '0' from freq '0'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '0' to 'period'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '0' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 3<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[3]: '3' to 'unexport'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip0/pwm3<br>
