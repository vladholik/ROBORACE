

struct tofs
{
  int l_90, l_45, l_0, r_0, r_45, r_90;
};

void TCA9548A(uint8_t bus);

bool setup_6_tofs(int timeout);

tofs tof_read();
