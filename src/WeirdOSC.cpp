#include "AudioDozer.hpp"

struct WeirdOSC : Module {
	enum ParamIds {
		VCO_KNOB_OSC1_FREQ,
		VCO_KNOB_OSC1_FINE,
		VCO_KNOB_OSC2_FREQ,
		VCO_KNOB_OSC2_FINE,
		VCO_KNOB_INDEX,
		MIX_KNOB_IN1,
		MIX_KNOB_IN2,
		MIX_KNOB_IN3,
		MIX_KNOB_SUM,
		VCF_KNOB_FREQ,
		VCF_KNOB_RES,
		VCA_KNOB_LEVEL,
		VCA_KNOB_BRR,
		LFO1_KNOB_FREQ,
		LFO2_KNOB_FREQ,
		ENV1_KNOB_ATK,
		ENV1_KNOB_DCY,
		ENV1_KNOB_REL,
		ENV1_KNOB_SUS,
		ENV2_KNOB_ATK,
		ENV2_KNOB_DCY,
		ENV2_KNOB_REL,
		ENV2_KNOB_SUS,
		NUM_PARAMS
	};

	enum InputIds {
		VCO_CV_IDX,
		MIV_VCC_IN3,
		MIX_VCC_IN2,
		MIX_VCC_IN1,
		MIX_CV_CV3,
		MIX_CV_CV2,
		MIX_CV_CV1,
		VCF_CV_IN,
		VCF_VCC_IN,
		VCO_CV_TRIG,
		VCA_VCC_IN,
		NUM_INPUTS
	};

	enum OutputIds {
		VCO_VCC_OSC1_SUM,
		VCO_VCC_OSC1_SQR,
		VCO_VCC_OSC1_SIN,
		VCO_VCC_OSC2_TRI,
		VCO_VCC_OSC2_SAW,
		VCO_VCC_OSC2_SIN,
		MIX_VCC_SUM,
		VCF_VCC_OUT,
		VCA_VCC_OUT,
		LFO1_VCC_SAW,
		LFO1_VCC_TRI,
		LFO1_VCC_SIN,
		LFO1_VCC_SQR,
		LFO1_VCC_RMP,
		LFO1_VCC_EXP,
		LFO2_VCC_NOI,
		LFO2_VCC_S_H,
		ENV1_CV_OUT,
		ENV2_CV_OUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};

	float phase = 0.0;
	float blinkPhase = 0.0;

	WeirdOSC() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		info("Init WeirdOSC...");
		info("Reset WeirdOSC...");
	}

	void step() override;
	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the 
	//   context menu
};

void WeirdOSC::step() {
	// Implement a simple sine oscillator

}

// Big Knob 	Davies1900hLargeBlack
// Small Knob  	Davies1900hBlack
// CV InOut 	CL1362
// 

struct WeirdOSCWidget : ModuleWidget {
	WeirdOSCWidget(WeirdOSC *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/WeirdOSC-Final-Test.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(ParamWidget::create<Davies1900hLargeBlackKnob>(Vec(42.37, 80.435), module, WeirdOSC::VCO_KNOB_OSC1_FREQ, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(165.49, 160.803), module, WeirdOSC::VCO_KNOB_OSC1_FINE, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hLargeBlackKnob>(Vec(158.39, 80.435), module, WeirdOSC::VCO_KNOB_OSC2_FREQ, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(50.47, 161.029), module, WeirdOSC::VCO_KNOB_OSC2_FINE, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(107.98, 125.213), module, WeirdOSC::VCO_KNOB_INDEX, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(294.73, 26.94), module, WeirdOSC::MIX_KNOB_IN1, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(295.33, 77.909), module, WeirdOSC::MIX_KNOB_IN2, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(295.33, 128.438), module, WeirdOSC::MIX_KNOB_IN3, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(295.33, 177.966), module, WeirdOSC::MIX_KNOB_SUM, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hLargeBlackKnob>(Vec(406.96, 28.767), module, WeirdOSC::VCF_KNOB_FREQ, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(416.06, 105.135), module, WeirdOSC::VCF_KNOB_RES, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hLargeBlackKnob>(Vec(505.86, 27.68), module, WeirdOSC::VCA_KNOB_LEVEL, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(514.97, 104.048), module, WeirdOSC::VCA_KNOB_BRR, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hLargeBlackKnob>(Vec(51.37, 242.795), module, WeirdOSC::LFO1_KNOB_FREQ, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hLargeBlackKnob>(Vec(224.11, 241.516), module, WeirdOSC::LFO2_KNOB_FREQ, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(313.15, 244.134), module, WeirdOSC::ENV1_KNOB_ATK, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(364.08, 244.494), module, WeirdOSC::ENV1_KNOB_DCY, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(364.96, 305.903), module, WeirdOSC::ENV1_KNOB_REL, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(312.56, 305.903), module, WeirdOSC::ENV1_KNOB_SUS, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(456.73, 244.368), module, WeirdOSC::ENV2_KNOB_ATK, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(507.66, 244.728), module, WeirdOSC::ENV2_KNOB_DCY, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(508.54, 306.136), module, WeirdOSC::ENV2_KNOB_REL, -3.0, 3.0, 0.0));
		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(456.13, 306.136), module, WeirdOSC::ENV2_KNOB_SUS, -3.0, 3.0, 0.0));

		addOutput(Port::create<CL1362Port>(Vec(206.01, 27.514), Port::OUTPUT, module, WeirdOSC::VCO_VCC_OSC1_SUM));
		addOutput(Port::create<CL1362Port>(Vec(170.20, 28.203), Port::OUTPUT, module, WeirdOSC::VCO_VCC_OSC1_SQR));
		addOutput(Port::create<CL1362Port>(Vec(135.29, 28.201), Port::OUTPUT, module, WeirdOSC::VCO_VCC_OSC1_SIN));
		addOutput(Port::create<CL1362Port>(Vec(88.05, 27.17), Port::OUTPUT, module, WeirdOSC::VCO_VCC_OSC2_TRI));
		addOutput(Port::create<CL1362Port>(Vec(52.24, 27.858), Port::OUTPUT, module, WeirdOSC::VCO_VCC_OSC2_SAW));
		addOutput(Port::create<CL1362Port>(Vec(17.33, 27.856), Port::OUTPUT, module, WeirdOSC::VCO_VCC_OSC2_SIN));
		addOutput(Port::create<CL1362Port>(Vec(255.46, 183.008), Port::OUTPUT, module, WeirdOSC::MIX_VCC_SUM));
		addOutput(Port::create<CL1362Port>(Vec(448.43, 164.208), Port::OUTPUT, module, WeirdOSC::VCF_VCC_OUT));
		addOutput(Port::create<CL1362Port>(Vec(546.94, 164.66), Port::OUTPUT, module, WeirdOSC::VCA_VCC_OUT));
		addOutput(Port::create<CL1362Port>(Vec(115.48, 307.492), Port::OUTPUT, module, WeirdOSC::LFO1_VCC_SAW));
		addOutput(Port::create<CL1362Port>(Vec(115.48, 274.964), Port::OUTPUT, module, WeirdOSC::LFO1_VCC_TRI));
		addOutput(Port::create<CL1362Port>(Vec(115.48, 242.437), Port::OUTPUT, module, WeirdOSC::LFO1_VCC_SIN));
		addOutput(Port::create<CL1362Port>(Vec(115.48, 340.019), Port::OUTPUT, module, WeirdOSC::LFO1_VCC_SQR));
		addOutput(Port::create<CL1362Port>(Vec(49.83, 340.436), Port::OUTPUT, module, WeirdOSC::LFO1_VCC_RMP));
		addOutput(Port::create<CL1362Port>(Vec(49.83, 308.662), Port::OUTPUT, module, WeirdOSC::LFO1_VCC_EXP));
		addOutput(Port::create<CL1362Port>(Vec(229.81, 338.674), Port::OUTPUT, module, WeirdOSC::LFO2_VCC_NOI));
		addOutput(Port::create<CL1362Port>(Vec(229.81, 306.9), Port::OUTPUT, module, WeirdOSC::LFO2_VCC_S_H));
		addOutput(Port::create<CL1362Port>(Vec(407.10, 312.12), Port::OUTPUT, module, WeirdOSC::ENV1_CV_OUT));
		addOutput(Port::create<CL1362Port>(Vec(553.63, 311.57), Port::OUTPUT, module, WeirdOSC::ENV2_CV_OUT));

		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::VCO_CV_IDX));
		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::MIV_VCC_IN3));
		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::MIX_VCC_IN2));
		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::MIX_VCC_IN1));
		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::MIX_CV_CV3));
		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::MIX_CV_CV2));
		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::MIX_CV_CV1));
		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::VCF_CV_IN));
		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::VCF_VCC_IN));
		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::VCO_CV_TRIG));
		addInput(Port::create<CL1362Port>(Vec(6, 272), Port::INPUT, module, WeirdOSC::VCA_VCC_IN));

		addChild(ModuleLightWidget::create<MediumLight<RedLight>>(Vec(41, 59), module, WeirdOSC::BLINK_LIGHT));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelWeirdOSC = Model::create<WeirdOSC, WeirdOSCWidget>("AudioDozer", 
															   "WeirdOSC", 
															   "WeirdOSC is All", 
															   OSCILLATOR_TAG);
