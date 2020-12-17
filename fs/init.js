load('api_timer.js');
load('api_config.js');
load('api_httplib.js');
load('api_sys.js');

let ua = Cfg.get('httplib.user_agent');

Timer.set(
    10000,
    true,
    function () {
        print('mJS.httlib Example/ Sys.uptime:', Sys.uptime());

        let url = Cfg.get('api_url'); //get URL from config
        let req = HTTPReq.create(url, 2, 'application/json');

        req.setBody(JSON.stringify({ values: [Sys.uptime(), Sys.free_ram()] }));
        req.addHeader('X-Hoge', 'abcdefg');

        let res = HTTPLib.send(req);
        print('res:', res.getStatus());
    },
    null
);
