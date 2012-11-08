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

end;

