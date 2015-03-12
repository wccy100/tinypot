#include "BigPotSubtitle.h"


BigPotSubtitle::BigPotSubtitle()
{

}


BigPotSubtitle::~BigPotSubtitle()
{
}

void BigPotSubtitle::init()
{
	_lib = ass_library_init();
	_ren = ass_renderer_init(_lib);
	ass_set_fonts(_ren, "c:\\windows\\fonts\\msyh.ttf", "Sans", 0, "", 0);
}

bool BigPotSubtitle::openSubtitle(const string& filename)
{
	//�����Ĳ�����char*,Ϊ�����⸴��һ��
	auto s = filename;
	_track = ass_read_file(_lib, (char*)s.c_str(), NULL);
	_haveSubtitle = (_track != nullptr);
	if (_haveSubtitle) _subfilename = filename;
	return 	_haveSubtitle;
}

void BigPotSubtitle::show(int time)
{
	int a;
	_img = ass_render_frame(_ren, _track, time, &a);
	//cout << engine_->getTicks() << endl;
	auto img = _img;
	if (a)
	{
		destroyAllTex();
		while (img)
		{
			auto t = engine_->transBitmapToTexture(img->bitmap, img->color, img->w, img->h, img->stride);
			engine_->renderCopy(t, img->dst_x, img->dst_y, img->w, img->h, 1);
			_tex_v.push_back(t);
			img = img->next;
		}
	}
	else
	{
		int i = 0;
		while (img && i < _tex_v.size())
		{
			engine_->renderCopy(_tex_v[i++], img->dst_x, img->dst_y, img->w, img->h, 1);
			img = img->next;
		}
	}
	//cout << engine_->getTicks() << endl;
}

void BigPotSubtitle::destroy()
{
	ass_renderer_done(_ren);
	ass_library_done(_lib);
}

void BigPotSubtitle::setFrameSize(int w, int h)
{
	//if (_track)
	ass_set_frame_size(_ren, w, h);
}

bool BigPotSubtitle::tryOpenSubtitle(const string& filename)
{
	string str;
	bool b = true;
	do
	{
		str = changeFileExt(filename, "srt");
		if (fileExist(str)) break;
		str = changeFileExt(filename, "ssa");
		if (fileExist(str)) break;
		str = changeFileExt(filename, "ass");
		if (fileExist(str)) break;
		str = fingFileWithMainName(filename);
		if (str != "") break;
		b = false;
	} while (false);

	if (str != "")
	{
		b = openSubtitle(str);
		printf("try load subtitle file %s, state %d\n", str.c_str(), b);
		
	}
	return b;
}

void BigPotSubtitle::destroyAllTex()
{
	for (auto t : _tex_v)
	{
		engine_->destroyTexture(t);
	}
	_tex_v.clear();
}

void BigPotSubtitle::closeSubtitle()
{
	if (_track)
		ass_free_track(_track);
}