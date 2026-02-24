enum Effect{

  None,
  Effect_0,
  Effect_1,
  Effect_2,
  Effect_3,
  Effect_4,
};
extern Effect current_effect;


struct Task
{
  uint16_t interval;
  void (*function)();
};
extern Task tasks[];



namespace effect0_scope {   // EFFECT 1 - random turn on all columns 

    void effect0(void);
}

namespace effect1_scope {   // EFFECT 2 - rain, random generated drops falling down 
  
    void effect1(void);
}

namespace effect2_scope {   // EFFECT 2 - fountain 
  
    void effect2(void);
}

namespace effect3_scope {   // EFFECT 3 - turn on leds one by one  
  
    void effect3(void);
}

namespace effect4_scope {   // EFFECT 3 - turn on leds one by one  
  
    void effect4(void);
}
