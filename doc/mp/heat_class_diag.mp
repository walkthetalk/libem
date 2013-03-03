
input metauml;

beginfig(1);
	pair pFSL, pFSH;
	Class.Heating("svcHeating")(
		"-m_svr: serveree",
		"-m_recver: recver",
		"-m_sender: sender",
		"-m_heater: heater",
		"-m_temp_sensor: temp_sensor",
		"-m_cfg: cfg_data_t",
		"-m_check_tmr: timeree",
		"-m_heat_tmr: timeree",
		"-m_ctx: heat_ctx",
		"-m_sm: heat_sm")(
		"-__svr_cb(serveree::args_t &)",
		"-__checkTmr_cb(timeree::args_t &)",
		"-__heatTmr_cb(timeree::args_t &)",
		"-__report(const std::string &)"
	);

	Class.poller("poller")()("+run()");
	poller.s = Heating.n + (0, 20);

	Class.cfgData("cfg_data_t")()();
	cfgData.w = Heating.e + (40,-65);

	Class.heater("heater")()();
	heater.w = cfgData.e + (10, 0);

	Class.temp_sensor("temp_sensor")()();
	temp_sensor.w = heater.e + (10,0);

	Class.timeree("timeree")()();
	timeree.w = temp_sensor.e + (10,0);

	Class.heatCtx("heat_ctx")()();
	heatCtx.s = ((cfgData.left + timeree.right)/2, cfgData.top + 40);

	Class.heatSm("heat_sm")()();
	heatSm.s = heatCtx.n + (0, 40);

	Class.serveree("serveree")()();
	serveree.s = heatSm.n + (0, 20);

	% draw
	drawObjects(Heating, poller, cfgData, heater, temp_sensor, timeree, heatCtx, heatSm, serveree);
	pFSL = (Heating.right+20, Heating.midy);
	link(compositionUni)(rpathStepY(pFSL, cfgData.s, -100)--Heating.e);
	link(compositionUni)(rpathStepY(pFSL, heater.s, -100)--Heating.e);
	link(compositionUni)(rpathStepY(pFSL, temp_sensor.s, -100)--Heating.e);
	link(compositionUni)(rpathStepY(pFSL, timeree.s, -100)--Heating.e);

	pFSH = heatCtx.s;
	link(aggregationUni)(rpathStepY(pFSH, cfgData.n, -20));
	link(aggregationUni)(rpathStepY(pFSH, heater.n, -20));
	link(aggregationUni)(rpathStepY(pFSH, temp_sensor.n, -20));
	link(aggregationUni)(rpathStepY(pFSH, timeree.n, -20));

	link(aggregationUni)(heatCtx.n -- heatSm.s);

	link(compositionUni)(rpathStepX(Heating.e, heatCtx.w, 20));
	link(compositionUni)(rpathStepX(Heating.e, heatSm.w, 20));
	link(compositionUni)(rpathStepX(Heating.e, serveree.w, 20));

	link(inheritance)(Heating.n -- poller.s);
endfig;

end;
