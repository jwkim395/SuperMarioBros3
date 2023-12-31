#include "pch.h"
#include "CCamera.h"

#include "CEngine.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CEditorLevel.h"
#include "CPlayLevel.h"
#include "CMario.h"

#include "CAssetMgr.h"
#include "CTexture.h"

CCamera::CCamera()
	: m_Veil(nullptr)
	, m_Alpha(0.f)	
{
	Vec2 vResol = CEngine::GetInst()->GetResolution();
	m_Veil = CAssetMgr::GetInst()->CreateTexture(L"VeilTex", vResol.x, vResol.y);
}

CCamera::~CCamera()
{
}

void CCamera::tick()
{
	// 화면 해상도의 중심위치를 알아낸다.
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2.f;
	if (dynamic_cast<CEditorLevel*>(CLevelMgr::GetInst()->GetCurLevel())) {
		if (KEY_TAP(KEY::LEFT))
		{
			m_vLookAt.x -= 64.f;
		}

		if (KEY_TAP(KEY::RIGHT))
		{
			m_vLookAt.x += 64.f;
		}

		if (KEY_TAP(KEY::UP))
		{
			m_vLookAt.y -= 64.f;
		}

		if (KEY_TAP(KEY::DOWN))
		{
			m_vLookAt.y += 64.f;
		}
		
	}
	if (dynamic_cast<CPlayLevel*>(CLevelMgr::GetInst()->GetCurLevel())) {
		if (nullptr != target) {
			Vec2 vResol = CEngine::GetInst()->GetResolution();
			m_vLookAt = target->GetPos();
			if (m_vLookAt.y + vResol.y / 2 > mapBottom.y) {
				m_vLookAt.y = mapBottom.y - vResol.y / 2;
			}
			if (m_vLookAt.x - vResol.x / 2 < mapLeft.x) {
				m_vLookAt.x = mapLeft.x + vResol.x / 2;
			}
			else if (m_vLookAt.x + vResol.x / 2 > mapRight.x) {
				m_vLookAt.x = mapRight.x - vResol.x / 2;
			}
		}
	}
	// 해상도 중심과, 카메라가 현재 보고있는 좌표의 차이값을 구한다.
	m_vDiff = m_vLookAt - vCenter;

	// 카메라 이벤트가 없으면 리턴
	if (m_EventList.empty())
		return;

	// 카메라 이벤트가 존재한다면
	FCamEvent& evnt = m_EventList.front();

	if (evnt.Type == CAM_EFFECT::FADE_IN)
	{
		evnt.AccTime += DT;

		if (evnt.Duration <= evnt.AccTime)
		{
			m_Alpha = 0;
			m_EventList.pop_front();
		}
		else
		{
			float fRatio = evnt.AccTime / evnt.Duration;
			float alpha = 1.f - fRatio;
			m_Alpha = (UINT)(alpha * 255);
		}
	}

	else if (evnt.Type == CAM_EFFECT::FADE_OUT)
	{
		evnt.AccTime += DT;

		if (evnt.Duration <= evnt.AccTime)
		{
			m_EventList.pop_front();
			m_Alpha = 255;
		}
		else
		{
			float fRatio = evnt.AccTime / evnt.Duration;
			float alpha = fRatio;
			m_Alpha = (UINT)(alpha * 255);
		}
	}
	

	
}

void CCamera::render(HDC _dc)
{
	if (0 == m_Alpha)
		return;

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;

	blend.SourceConstantAlpha = m_Alpha; // 0 ~ 255
	blend.AlphaFormat = 0; // 0

	AlphaBlend(_dc
		, 0, 0, m_Veil->GetWidth(), m_Veil->GetHeight()
		, m_Veil->GetDC()
		, 0, 0
		, m_Veil->GetWidth(), m_Veil->GetHeight()
		, blend);
}