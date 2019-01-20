#include <math.h>
#include <stdlib.h>

/// sample output type (16-bit for PCM)
typedef short SampleValue;
/// 8-bit data type
typedef char  bsInt8;
/// 16-bit data type 
typedef short bsInt16;
/// 32-bit data type 
typedef int   bsInt32;
/// 8-bit unsigned type
typedef unsigned char  bsUint8;
/// 16-bit unsigned type
typedef unsigned short bsUint16;
/// 32-bit unsigned type
typedef unsigned int   bsUint32;
/// transparent data type
typedef void* Opaque;

// Choose one of these as the oscillator phase accumulator type.
// double gives less noise with slightly more calculation time.
/// Type for a phase accumulator
typedef double PhsAccum;
#define sinv sin
//typedef float PhsAccum;
//#define sinv sinf

/// Type for an amplitude value
typedef float AmpValue;
/// Type for a high precision amplitude value
typedef double AmpValue2;
/// Type for a frequency or time value
typedef float FrqValue;
/// Type for a generic parameter
typedef float ParamValue;

#define PI 3.14159265358979
#define twoPI 6.28318530717958

#define PANTBLLEN 4096
#define MAX_AMPCB 1440

#define oneDivPI (1.0/PI)
#define twoDivPI (2.0/PI)

typedef struct _GenWave {
    PhsAccum indexIncr;
    PhsAccum index;
    FrqValue frq;
    /// pre-calculated multipler for frequency to radians (twoPI/sampleRate)
	PhsAccum frqRad;
    /// Sample rate
	FrqValue sampleRate;
} GenWave;

void genwave_setfreq(GenWave *gw, FrqValue f) {
    gw->frq = f;
}

GenWave *genwave_init(FrqValue sr) {
    GenWave *gw = (GenWave *) malloc(sizeof(GenWave));
    
    if (!gw)
        return NULL;
    
    gw->sampleRate = sr;
    gw->indexIncr = 0;
    gw->index = 0;
    gw->frq = 440;
    gw->frqRad = twoPI / (PhsAccum) gw->sampleRate;

    return gw;
}

/// Reset the oscillator. The phase increment is calculated based on the 
/// last set frequency value. The phase argument indicates the next phase.
/// When set to 0, the oscillator is reset to the initial conditions. Values
/// greater than 0 cause calculation of the appropriate starting sample.
/// Values less than zero cause the phase to remain unchanged.
/// @param initPhs phase in radians
void genwave_reset(GenWave *gw, float initPhs = 0)
{
	gw->indexIncr = (PhsAccum) gw->frq * gw->frqRad;
	if (gw->indexIncr > PI)
		gw->indexIncr = PI;
	if (initPhs >= 0)
		gw->index = initPhs;
}

inline PhsAccum genwave_phasewrap(PhsAccum index)
{
	while (index >= twoPI)
		index -= twoPI;
	while (index < 0)
		index += twoPI;
	return index;
}

/// Generate the next sample. The sample amplitude is normalized to [-1,+1]
/// range. The caller must apply any amplitude peak level multiplier.
/// @return sample value for the current phase
AmpValue genwave_sin_next_sample(GenWave *gw)
{
	gw->index = genwave_phasewrap(gw->index);
	AmpValue out = sinv(gw->index);
	gw->index += gw->indexIncr;
	return out;
}

/// Modulate the oscillator phase. This changes the oscillator frequency by
/// directly altering the current oscillator phase. This is faster than
/// Modulate() if the caller has pre-calculated the phase range.
/// @param phs delta phase in radians
void genwave_phasemod(GenWave *gw, PhsAccum phs)
{
	gw->index += phs;
}

void genwave_destroy(GenWave *gw) {
    if (gw)
        free(gw);
}

/// Return the next sample. The generated value is multiplied by the
/// supplied input value.
/// @param in sample peak amplitude
AmpValue genwave_sin_generate(GenWave *gw, AmpValue in)
{
	return genwave_sin_next_sample(gw) * in;
}

/// Modulate the oscillator frequency.  This forces recalculation of the
/// phase increment by adding the argument to the last set frequency value.
/// @param d delta frequency in Hz
void genwave_sin_modulate(GenWave *gw, FrqValue d)
{
	gw->indexIncr = (PhsAccum)(gw->frq + d) * gw->frqRad;
	if (gw->indexIncr > PI)
		gw->indexIncr = PI;
}

/// Triangle wave by direct calculation. This is very fast, but is not
/// bandwidth limited. 
void genwave_tri_modulate(GenWave *gw, FrqValue d)
{
	gw->indexIncr = (PhsAccum)(gw->frq + d) * gw->frqRad;
	if (gw->indexIncr >= PI)
		gw->indexIncr -= twoPI;
	else if (gw->indexIncr < -PI)
		gw->indexIncr += twoPI;
}

/// @copydoc GenWave::PhaseMod()
void genwave_tri_phasemod(GenWave *gw, PhsAccum phs)
{
	gw->index += phs;
	if (gw->index >= PI)
		gw->index -= twoPI;
	else if (gw->index < -PI)
		gw->index += twoPI;
}

/// @note phase index varies from [-PI, PI] not [0, 2PI]
AmpValue genwave_tri_next_sample(GenWave *gw)
{
	//AmpValue triValue = (AmpValue)(1 + (2 * fabs(index - PI) / PI);
	AmpValue triValue = (AmpValue)(gw->index * twoDivPI);
	if (triValue < 0)
		triValue = 1.0 + triValue;
	else
		triValue = 1.0 - triValue;
	if ((gw->index += gw->indexIncr) >= PI)
		gw->index -= twoPI;
	return triValue;
}

AmpValue genwave_tri_generate(GenWave *gw, AmpValue in)
{
	return genwave_tri_next_sample(gw) * in;
}

