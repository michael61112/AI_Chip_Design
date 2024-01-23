#include "ROM.h"

void ROM::run()
{
	int layer_num;
	int w_size;
	int pe_num;
	int o_channel_size;
	int cnt;
	Flit flit;

	o_finish_flg->write(0);
	while(!finish)
	{
		if(i_param_size_info->num_available())
		{
			param_sizes.push_back(i_param_size_info->read());
		}
		else if(i_load_param_flg->read())
		{
			cnt = 0;
			cout << "<Info> Start loading parameters" << endl;
			layer_num = param_sizes.size();
			for(int i=0; i<layer_num; i++)
			{
				w_size = param_sizes[i].H;
				pe_num = param_sizes[i].W;
				o_channel_size = param_sizes[i].C;
				if(w_size == 0)
					continue;
				// output weight
				flit.layer_id = i;
				flit.type = WEIGHT;
				for(int c=0; c<o_channel_size; c++)
				{
					for(int pe=0; pe<pe_num; pe++)
					{
						for(int w=0; w<w_size; w++)
						{
							flit.PE_id = pe;
							flit.data = rom_W[cnt];
							o_data->write(flit);
							wait();
							cnt++;
						}
					}
				}
				// output bias
				flit.type = BIAS;
				flit.PE_id = 0;
				for(int c=0; c<o_channel_size; c++)
				{
					flit.data = rom_W[cnt];
					o_data->write(flit);
					wait();
					cnt++;
				}
			}
			o_finish_flg->write(1);
			wait();
			o_finish_flg->write(0);
		}
		else if(i_load_input_flg->read())
		{
			flit.type = INPUT;
			flit.layer_id = 0;
			for(int i=0; i<img_channel; i++)
			{
				flit.PE_id = i;
				for(int j=0; j<img_size; j++)
				{
					flit.data = rom_I[i*img_size+j];
					o_data->write(flit);
					wait();
				}
			}
			o_finish_flg->write(1);
			wait();
			o_finish_flg->write(0);
			finish = true;
		}
		wait();
	}
}

void ROM::readWeightFile(string file_name)
{
	ifstream f;
	string param_name;
	int param_num;
	int cnt = 0;
	
	f.open(file_name);
    while(f >> param_name)
	{
		f >> param_num;
		param_nums.push_back(param_num);
		if(cnt+param_num > W_MEM_SIZE)
		{
			cout << "<Error> W_MEM_SIZE should set larger." << endl;
			load_data_fail = true;
			return;
		}
		for(int i=0; i<param_num; i++)
		{
			f >> rom_W[cnt];
			cnt++;
		}
	}
	f.close();
}

void ROM::readInputFile(string file_name)
{
	ifstream f;
	
	f.open(file_name);
	f >> img_size >> img_channel >> img_num;
	if(img_size*img_channel > I_MEM_SIZE)
	{
		cout << "<Error> I_MEM_SIZE should set larger." << endl;
		load_data_fail = true;
		return;
	}
    for(int i=0; i<img_size*img_channel; i++)
	{
		f >> rom_I[i];
        if(i < img_size*img_channel) // print first image
		{
			if(rom_I[i] > 0)  cout << "##";
			else              cout << "  ";
			if((i+1) % 32 == 0)
				cout << endl;
		}
	}
	f.close();
}
