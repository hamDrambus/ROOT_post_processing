{
if (false) {
	channel_info<dB_info> atten0;
	atten0.push(0, dB_info(12)); //decibells, not ratio
	atten0.push(1, dB_info(12));
	dBs["180705_Cd_20kV_800V_12bB_48V"] = atten0;
}
if (false) {
	int channel = 0;
	bool display = true;
	int aggressiveness = 0;	
	if (aggressiveness !=1) {
    	cut_A_S_upper(1.4, 0, 1e2, 0, display, channel, "rem_A>1.4");
		cut_A_S_upper(0.0, 0.55, 1.4, 1.1, display, channel, "rem_S>~0.7");
	}
}
	if (true) {
		analysis_history(false, 0); //20
		nex();
		analysis_history(false, 0); //18
		nex();
		analysis_history(false, 0); //16
		nex();
		analysis_history(false, 0); //14
		nex();
		analysis_history(false, 0); //13
		nex();
		analysis_history(false, 0); //12
		nex();
		analysis_history(false, 0); //11
		nex();
		analysis_history(false, 0); //10
		nex();
		analysis_history(false, 0); //9
		nex();
		analysis_history(false, 0); //8
		nex();
	}
} 
