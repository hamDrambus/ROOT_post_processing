 {
	if (!post_processor->is_PMT_type(post_processor->current_type)) {
		std::map<int, std::pair<double, double> > cut_vals;
		if (post_processor->experiments[post_processor->current_exp_index] == "190404_Cd_20kV_850V_46V_th250mV") {
			//SiPM (MPPC) channel - (Smin, Smax). Set by hand for each experiment and channel.			
cut_vals[32] = std::pair<double, double> (0.0009, 0.007);
cut_vals[33] = std::pair<double, double> (0.0007, 0.006);
cut_vals[34] = std::pair<double, double> (0.001, 0.008);
cut_vals[35] = std::pair<double, double> (0.001, 0.008);
cut_vals[36] = std::pair<double, double> (0.001, 0.008);
cut_vals[37] = std::pair<double, double> (0.0009, 0.005);
cut_vals[38] = std::pair<double, double> (0.001, 0.01);
cut_vals[39] = std::pair<double, double> (0.001, 0.008);
cut_vals[40] = std::pair<double, double> (0.0009, 0.009);
cut_vals[41] = std::pair<double, double> (0.0009, 0.009);
cut_vals[42] = std::pair<double, double> (0.0009, 0.007);
cut_vals[43] = std::pair<double, double> (-1.0, 0.0); //no contact for this channel, ignore it
cut_vals[44] = std::pair<double, double> (0.0008, 0.008);
cut_vals[48] = std::pair<double, double> (0.001, 0.008);
cut_vals[49] = std::pair<double, double> (0.0007, 0.006);
cut_vals[50] = std::pair<double, double> (0.00075, 0.006);
cut_vals[51] = std::pair<double, double> (0.0009, 0.008);
cut_vals[52] = std::pair<double, double> (0.001, 0.007);
cut_vals[53] = std::pair<double, double> (0.001, 0.011);
cut_vals[54] = std::pair<double, double> (0.0009, 0.006);
cut_vals[55] = std::pair<double, double> (0.0009, 0.007);
cut_vals[56] = std::pair<double, double> (0.001, 0.008);
cut_vals[57] = std::pair<double, double> (0.0009, 0.009);
cut_vals[58] = std::pair<double, double> (0.001, 0.007);
cut_vals[59] = std::pair<double, double> (0.001, 0.006);
		}
		if (post_processor->experiments[post_processor->current_exp_index] == "190404_Cd_20kV_850V_46V_th250mV_0") {
			//SiPM (MPPC) channel - (Smin, Smax). Set by hand for each experiment and channel.			
cut_vals[32] = std::pair<double, double> (0.0009, 0.007);
cut_vals[33] = std::pair<double, double> (0.00075, 0.006);
cut_vals[34] = std::pair<double, double> (0.0009, 0.01);
cut_vals[35] = std::pair<double, double> (0.001, 0.008);
cut_vals[36] = std::pair<double, double> (0.001, 0.009);
cut_vals[37] = std::pair<double, double> (0.00085, 0.005);
cut_vals[38] = std::pair<double, double> (0.001, 0.01);
cut_vals[39] = std::pair<double, double> (0.001, 0.007);
cut_vals[40] = std::pair<double, double> (0.0009, 0.009);
cut_vals[41] = std::pair<double, double> (0.0009, 0.009);
cut_vals[42] = std::pair<double, double> (0.001, 0.007);
cut_vals[43] = std::pair<double, double> (-1.0, 0.0); //no contact for this channel, ignore it
cut_vals[44] = std::pair<double, double> (0.0008, 0.007);
cut_vals[48] = std::pair<double, double> (0.0009, 0.008);
cut_vals[49] = std::pair<double, double> (0.0007, 0.007);
cut_vals[50] = std::pair<double, double> (0.0007, 0.006);
cut_vals[51] = std::pair<double, double> (0.0008, 0.007);
cut_vals[52] = std::pair<double, double> (0.001, 0.01);
cut_vals[53] = std::pair<double, double> (0.001, 0.01);
cut_vals[54] = std::pair<double, double> (0.0008, 0.008);
cut_vals[55] = std::pair<double, double> (0.0009, 0.008);
cut_vals[56] = std::pair<double, double> (0.00095, 0.008);
cut_vals[57] = std::pair<double, double> (0.001, 0.01);
cut_vals[58] = std::pair<double, double> (0.001, 0.008);
cut_vals[59] = std::pair<double, double> (0.001, 0.009);
		}
		if (post_processor->experiments[post_processor->current_exp_index] == "190404_Cd_18kV_850V_46V_th230mV") {
			//SiPM (MPPC) channel - (Smin, Smax). Set by hand for each experiment and channel.			
cut_vals[32] = std::pair<double, double> (0.0009, 0.008);
cut_vals[33] = std::pair<double, double> (0.0007, 0.006);
cut_vals[34] = std::pair<double, double> (0.001, 0.009);
cut_vals[35] = std::pair<double, double> (0.001, 0.008);
cut_vals[36] = std::pair<double, double> (0.001, 0.009);
cut_vals[37] = std::pair<double, double> (0.00085, 0.007);
cut_vals[38] = std::pair<double, double> (0.001, 0.01);
cut_vals[39] = std::pair<double, double> (0.001, 0.009);
cut_vals[40] = std::pair<double, double> (0.00095, 0.0085);
cut_vals[41] = std::pair<double, double> (0.0009, 0.008);
cut_vals[42] = std::pair<double, double> (0.0009, 0.009);
cut_vals[43] = std::pair<double, double> (-1.0, 0.0); //no contact for this channel, ignore it
cut_vals[44] = std::pair<double, double> (0.0008, 0.007);
cut_vals[48] = std::pair<double, double> (0.0009, 0.008);
cut_vals[49] = std::pair<double, double> (0.0007, 0.0055);
cut_vals[50] = std::pair<double, double> (0.00075, 0.006);
cut_vals[51] = std::pair<double, double> (0.001, 0.007);
cut_vals[52] = std::pair<double, double> (0.001, 0.01);
cut_vals[53] = std::pair<double, double> (0.001, 0.01);
cut_vals[54] = std::pair<double, double> (0.0008, 0.008);
cut_vals[55] = std::pair<double, double> (0.0008, 0.009);
cut_vals[56] = std::pair<double, double> (0.001, 0.008);
cut_vals[57] = std::pair<double, double> (0.0009, 0.009);
cut_vals[58] = std::pair<double, double> (0.001, 0.009);
cut_vals[59] = std::pair<double, double> (0.0009, 0.009);
		}
		if (post_processor->isMultichannel(post_processor->current_type)) {
			for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
				int chan = post_processor->MPPC_channels[ich];
				if (cut_vals.end()!=cut_vals.find(chan))
					cut_S(cut_vals.find(chan)->second.first, cut_vals.find(chan)->second.second, false, chan);
			}
		} else {
			for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
				int chan = post_processor->MPPC_channels[ich];
				if (cut_vals.end()!=cut_vals.find(chan)) {
					ch(chan);
					cut_S(cut_vals.find(chan)->second.first, cut_vals.find(chan)->second.second, false);
				}
			}
		}		
	} else { //PMT
		std::map<int, std::pair<double, double> > cut_vals;
		if (post_processor->experiments[post_processor->current_exp_index] == "190404_Cd_20kV_850V_46V_th250mV") {
			//SiPM (MPPC) channel - (Smin, Smax). Set by hand for each experiment and channel.			
cut_vals[8] = std::pair<double, double> (0., 0.);
cut_vals[9] = std::pair<double, double> (0., 0.);
cut_vals[10] = std::pair<double, double> (0., 0.);
cut_vals[11] = std::pair<double, double> (0., 0.);
cut_vals[12] = std::pair<double, double> (0.0, 0.5);
cut_vals[13] = std::pair<double, double> (0.0, 0.5);
cut_vals[14] = std::pair<double, double> (0.0, 0.5);
cut_vals[15] = std::pair<double, double> (0.0, 0.5);
cut_vals[16] = std::pair<double, double> (0.0, 0.5);
		}
		if (post_processor->experiments[post_processor->current_exp_index] == "190404_Cd_20kV_850V_46V_th250mV_0") {
			//SiPM (MPPC) channel - (Smin, Smax). Set by hand for each experiment and channel.			
cut_vals[8] = std::pair<double, double> (0., 0.);
cut_vals[9] = std::pair<double, double> (0., 0.);
cut_vals[10] = std::pair<double, double> (0., 0.);
cut_vals[11] = std::pair<double, double> (0., 0.);
cut_vals[12] = std::pair<double, double> (0., 0.);
cut_vals[13] = std::pair<double, double> (0., 0.);
cut_vals[14] = std::pair<double, double> (0., 0.);
cut_vals[15] = std::pair<double, double> (0., 0.);
cut_vals[16] = std::pair<double, double> (0., 0.);
		}
		if (post_processor->experiments[post_processor->current_exp_index] == "190404_Cd_18kV_850V_46V_th230mV") {
			//SiPM (MPPC) channel - (Smin, Smax). Set by hand for each experiment and channel.			
cut_vals[8] = std::pair<double, double> (0., 0.);
cut_vals[9] = std::pair<double, double> (0., 0.);
cut_vals[10] = std::pair<double, double> (0., 0.);
cut_vals[11] = std::pair<double, double> (0., 0.);
cut_vals[12] = std::pair<double, double> (0., 0.);
cut_vals[13] = std::pair<double, double> (0., 0.);
cut_vals[14] = std::pair<double, double> (0., 0.);
cut_vals[15] = std::pair<double, double> (0., 0.);
cut_vals[16] = std::pair<double, double> (0., 0.);
		}
		if (post_processor->isMultichannel(post_processor->current_type)) {
			for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
				int chan = post_processor->MPPC_channels[ich];
				if (cut_vals.end()!=cut_vals.find(chan))
					cut_S(cut_vals.find(chan)->second.first, cut_vals.find(chan)->second.second, false, chan);
			}
		} else {
			for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
				int chan = post_processor->MPPC_channels[ich];
				if (cut_vals.end()!=cut_vals.find(chan)) {
					ch(chan);
					cut_S(cut_vals.find(chan)->second.first, cut_vals.find(chan)->second.second, false);
				}
			}
		}
	}
 }
