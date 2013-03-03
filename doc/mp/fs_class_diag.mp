
input metauml;

beginfig(1);
	pair pFSL, pFSH;
	Class.fusionSplicer("svcFusionSplicing")(
		"-m_svr: serveree",
		"-m_recver: recver",
		"-m_sender: sender",
		"-m_camera: camera",
		"-m_motorLX: motor",
		"-m_motorLY: motor",
		"-m_motorRX: motor",
		"-m_motorRY: motor",
		"-m_hvb: hvb",
		"-m_cfg: cfg_data_t",
		"-m_ctx: fs_ctx",
		"-m_sm: fs_sm")(
		"-__svr_cb(serveree::args_t &)",
		"-__motorLX_cb(motor::args_t &)",
		"-__motorLY_cb(motor::args_t &)",
		"-__motorRX_cb(motor::args_t &)",
		"-__motorRY_cb(motor::args_t &)",
		"-__report(const std::string &)"
	);

	Class.poller("poller")()("+run()");
	poller.s = fusionSplicer.n + (0, 20);

	Class.cfgData("cfg_data_t")()();
	cfgData.w = fusionSplicer.e + (30,-65);

	Class.motor("motor")()();
	motor.w = cfgData.e + (20, 0);

	Class.camera("camera")()();
	camera.w = motor.e + (20,0);

	Class.hvb("hvb")()();
	hvb.w = camera.e + (20,0);

	Class.fsCtx("fs_ctx")()();
	fsCtx.s = ((cfgData.left + hvb.right)/2, cfgData.top + 40);

	Class.fsSm("fs_sm")()();
	fsSm.s = fsCtx.n + (0, 40);

	Class.serveree("serveree")()();
	serveree.s = fsSm.n + (0, 20);

	% draw
	drawObjects(fusionSplicer, poller, cfgData, motor, camera, hvb, fsCtx, fsSm, serveree);

	pFSL = (fusionSplicer.right+20, fusionSplicer.midy);
	link(compositionUni)(rpathStepY(pFSL, cfgData.s, -100)--fusionSplicer.e);
	link(compositionUni)(rpathStepY(pFSL, motor.s, -100)--fusionSplicer.e);
	link(compositionUni)(rpathStepY(pFSL, camera.s, -100)--fusionSplicer.e);
	link(compositionUni)(rpathStepY(pFSL, hvb.s, -100)--fusionSplicer.e);

	pFSH = fsCtx.s;
	link(aggregationUni)(rpathStepY(pFSH, cfgData.n, -20));
	link(aggregationUni)(rpathStepY(pFSH, motor.n, -20));
	link(aggregationUni)(rpathStepY(pFSH, camera.n, -20));
	link(aggregationUni)(rpathStepY(pFSH, hvb.n, -20));

	link(aggregationUni)(fsCtx.n -- fsSm.s);

	link(compositionUni)(rpathStepX(fusionSplicer.e, fsCtx.w, 20));
	link(compositionUni)(rpathStepX(fusionSplicer.e, fsSm.w, 20));
	link(compositionUni)(rpathStepX(fusionSplicer.e, serveree.w, 20));

	link(inheritance)(fusionSplicer.n -- poller.s);
endfig;

end;
