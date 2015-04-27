#include "BigPotStreamVideo.h"


BigPotStreamVideo::BigPotStreamVideo()
{
	//��Ƶ������, �㹻��ʱ���������������֡����˸
	maxSize_ = 0;
}


BigPotStreamVideo::~BigPotStreamVideo()
{
}

//-1����Ƶ
//1�п���ʾ�İ���δ��ʱ��
//2�Ѿ�û�п���ʾ�İ�
int BigPotStreamVideo::showTexture(int time)
{
	if (stream_index_ < 0)
		return -1;
	if (haveDecoded())
	{
		auto f = getCurrentFrameData();
		int time_c = f.time;
		if (time >= time_c)
		{
			auto tex = (BP_Texture*)f.data;
			engine_->renderCopy(tex);
			time_shown_ = time_c;
			ticks_shown_ = engine_->getTicks();
			dropDecoded();
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return 2;	
}

void BigPotStreamVideo::freeData(void* p)
{
	engine_->destroyTexture((BP_Texture*)p);
}

BigPotStream::FrameData BigPotStreamVideo::convert(void* p /*= nullptr*/)
{
	auto &f = frame_;
	auto tex = (BP_Texture*)data_;
	if (useMap())
	{
		tex = engine_->createYUVTexture(codecCtx_->width, codecCtx_->height);
	}
	engine_->updateYUVTexture(tex, f->data[0], f->linesize[0], f->data[1], f->linesize[1], f->data[2], f->linesize[2]);
	return{ time_dts_, f->linesize[0], tex };
}

int BigPotStreamVideo::dropTexture()
{
	return 0;
}