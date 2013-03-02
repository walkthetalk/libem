%
% author:	Ni Qingliang
% date:		2011-02-11
%
input metauml;

% begin figure 1
beginfig(1);
	Class.poller("poller")()("+run()");
	Class.pollee("pollee")()("+dispose()");
	Class.serveree("serveree")()();
	Class.connectee("connectee")()();
	Class.clientee("clientee")()();
	Class.timeree("timeree")()();

	Note.noteMethodWithPollee("pollee->dispose()");

	% layout
	pollee.w = poller.e + (100, 0);
	serveree.ne = pollee.s - (60, 50);

	connectee.nw = serveree.ne + (10, 0);

	clientee.nw = connectee.ne + (10, 0);

	timeree.nw = clientee.ne + (10, 0);

	noteMethodWithPollee.n = poller.sw - (10, 20);

	% draw
	drawObjects(poller, pollee, serveree, connectee, clientee, timeree, noteMethodWithPollee);

	clink(aggregationUni)(pollee, poller);
	link(inheritance)(pathStepY(serveree.n, pollee.s, 20));
	link(inheritance)(pathStepY(connectee.n, pollee.s, 20));
	link(inheritance)(pathStepY(clientee.n, pollee.s, 20));
	link(inheritance)(pathStepY(timeree.n, pollee.s, 20));

	link(dashedLink)(poller.methodStack.pict[0].w -- noteMethodWithPollee.n);

endfig;

beginfig(2);
	Begin.rb;
	End.re;

	State.entering("Entering")();
	State.pushOne("Push1")();
	State.clearing("Clearing")();
	State.defectDetecting("DefectDetecting")();
	State.pushTwo("Push2")();
	State.pauseOne("Pause1")();
	State.calibrating("Calibrating")();
	State.pauseTwo("Pause2")();
	State.preSplicing("PreSplicing")();
	State.splicing("Splicing")();
	State.lossEstimating("LossEstimating")();
	State.storing("Storing")();

	State.running("Running")(rb, re, entering, pushOne, clearing, defectDetecting, pushTwo, pauseOne, calibrating, pauseTwo, preSplicing, splicing, lossEstimating, storing);
	entering.midx = rb.midx;
	entering.top = rb.bottom - 20;

	pushOne.midx = entering.midx;
	pushOne.top = entering.bottom - 20;

	clearing.midx = pushOne.midx;
	clearing.top = pushOne.bottom - 20;

	defectDetecting.midx = clearing.midx;
	defectDetecting.top = clearing.bottom - 20;

	pushTwo.midy = defectDetecting.midy;
	pushTwo.left = defectDetecting.right + 20;

	pauseOne.midx = pushTwo.midx;
	pauseOne.bottom = pushTwo.top + 20;

	calibrating.midx = pauseOne.midx;
	calibrating.bottom = pauseOne.top + 20;

	pauseTwo.midx = calibrating.midx;
	pauseTwo.bottom = calibrating.top + 20;

	preSplicing.midy = pauseTwo.midy;
	preSplicing.left = pauseTwo.right + 20;

	splicing.midx = preSplicing.midx;
	splicing.top = preSplicing.bottom - 20;

	lossEstimating.midx = splicing.midx;
	lossEstimating.top = splicing.bottom - 20;

	storing.midx = lossEstimating.midx;
	storing.top = lossEstimating.bottom - 20;

	re.midx = storing.midx;
	re.top = storing.bottom - 20;

	running.info.drawNameLine := 1;
	drawObject(running);
	link(transition)(rb.s -- entering.n);
	link(transition)(entering.s -- pushOne.n);
	link(transition)(pushOne.s -- clearing.n);
	link(transition)(clearing.s -- defectDetecting.n);
	link(transition)(defectDetecting.e -- pushTwo.w);
	link(transition)(pushTwo.n -- pauseOne.s);
	link(transition)(pauseOne.n -- calibrating.s);
	link(transition)(calibrating.n -- pauseTwo.s);
	link(transition)(pauseTwo.e -- preSplicing.w);
	link(transition)(preSplicing.s -- splicing.n);
	link(transition)(splicing.s -- lossEstimating.n);
	link(transition)(lossEstimating.s -- storing.n);
	link(transition)(storing.s -- re.n);

	Begin.b;
	End.e;
	State.idle("Idle")();

	idle.midy = running.midy;
	idle.right = running.left - 40;

	b.midx = idle.midx;
	b.bottom = idle.top + 60;

	e.midx = idle.midx;
	e.top = idle.bottom - 60;

	drawObject(idle);
	drawObject(b);
	drawObject(e);

	link(transition)(b.s -- idle.n);
	link(transition)(idle.s -- e.n);
	draw image(link(transition)(idle.e -- running.w)) shifted(0,5);
	draw image(link(transition)(running.w -- idle.e)) shifted(0,-5);
	link(transition)((running.left,e.midy) -- e.e);
endfig;

end;

