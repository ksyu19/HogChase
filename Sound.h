// Sound.h
// Runs on TM4C123 or LM4F120
// Prototypes for basic functions to play sounds from the
// original Space Invaders.
// Jonathan Valvano
// November 17, 2014

void Play (void);
void Sound_Init(void);
void Sound_Play(const uint8_t *pt, uint32_t count);
void Music_Play(const uint8_t *pt, uint32_t count);
void Music_Stop(void);
void Sound_Smoke(void);
void Sound_Killed(void);
void Sound_Collect(void);
void Sound_Music(void);

