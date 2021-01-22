# mgos-httplibの example C言語 実装

BME280から取得したセンサーデータを送信する。

参考: <https://github.com/BMandW/mgos-httplib>

- 3分間隔でBME280から温湿度データを取得し指定したURLに送信する。
- configから送信先URLを取得
- カスタムリクエストヘッダの設定
- JSONリクエストボディの設定
    - リクエストオブジェクトのbodyをバッファにjson_printfをする
- リクエストの送信
- レスポンスヘッダ値の取得
- JSONレスポンスのparse


## config

- httplib.user_agent: httplibのUser-Agent
- api_url: 送信先URL
- i2c.sda_gpio: 21: BME280で使用するGPIO
- i2c.scl_gpio: 22: BME280で使用するGPIO
