load('api_timer.js');
load('api_config.js');
load('api_httplib.js');
load('api_sys.js');

let ua = Cfg.get('httplib.user_agent');

function timer_handler() {
    print('mJS.httlib Example/ Sys.uptime:', Sys.uptime());

    let url = Cfg.get('api_url'); //get URL from config

    let req = HTTPReq.create(url, HTTPLib.MT_POST, HTTPLib.CT_JSON);

    req.setBody(JSON.stringify({ values: [Sys.uptime(), Sys.free_ram()] }));
    req.addHeader('X-Imprement', 'mJS');

    let res = HTTPLib.send(req);
    if (res.isSuccess()) {
        print('Status');
        print(res.getStatus());
        print('Header');
        print(res.getHeader());
        print('Body');
        print(res.getBody());
    } else {
        print('HTTP Request Error:', res.getStatus());
    }
    res.free();
}

//Timer.set(10000, true, timer_handler, null);
