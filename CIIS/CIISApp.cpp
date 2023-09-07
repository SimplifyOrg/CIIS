#include "CIISApp.h"
#include "CIISFrame.h"

bool CIISApp::OnInit() {
	CIISFrame* frame = new CIISFrame();
	frame->SetClientSize(800, 600);
	frame->Center();
	frame->Show(true);
	return true;
}
