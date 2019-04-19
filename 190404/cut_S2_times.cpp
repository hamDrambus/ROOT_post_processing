 {
	if (!post_processor->is_PMT_type(post_processor->current_type)) {
		double from = 25;
		double to = 70;	//20kV
		to = 65; //18 kV
		to = 70; //16 kV
		to = 70; //14 kV
		to = 70; //12 kV
		to = 80; //10 kV
		to = 80; //8 kV
		if (post_processor->isMultichannel(post_processor->current_type)) {
			for (int ich =0; ich!= post_processor->MPPC_channels.size(); ++ich) {
				int chan = post_processor->MPPC_channels[ich];
				cut_S_t_rect_select(from, to, 0, 1e5, false, chan);
			}
		} else {
			cut_S_t_rect_select(from, to, 0, 1e5, false);
		}		
	} else { //PMT
		double from = 25;
		double to = 70;
		to = 65; //18 kV
		to = 70; //16 kV
		to = 70; //14 kV
		to = 70; //12 kV
		to = 80; //10 kV
		to = 80; //8 kV
		if (post_processor->isMultichannel(post_processor->current_type)) {
			for (int ich =0; ich!= post_processor->PMT_channels.size(); ++ich) {
				int chan = post_processor->PMT_channels[ich];
				cut_S_t_rect_select(from, to, 0, 1e5, false, chan);
			}
		} else {
			cut_S_t_rect_select(from, to, 0, 1e5, false);
		}
	}
 }
