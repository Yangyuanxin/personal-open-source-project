远程情书打印机json模板

```json
{
  "version": "1.0",
  "profile": {
    "ProductId": "WONYDFWVJO",
    "CategoryId": "539"
  },
  "properties": [
    {
      "id": "power_switch",
      "name": "电灯开关",
      "desc": "控制灯光",
      "required": true,
      "mode": "rw",
      "define": {
        "type": "bool",
        "mapping": {
          "0": "关灯",
          "1": "开灯"
        }
      }
    },
    {
      "id": "power_switch1",
      "name": "浪漫彩灯",
      "desc": "控制彩灯",
      "required": true,
      "mode": "rw",
      "define": {
        "type": "bool",
        "mapping": {
          "0": "关彩灯",
          "1": "开彩灯"
        }
      }
    },
    {
      "id": "lx",
      "name": "光照强度",
      "desc": "光照强度",
      "mode": "r",
      "define": {
        "type": "int",
        "min": "0",
        "max": "20000",
        "start": "0",
        "step": "1",
        "unit": "lx"
      },
      "required": false
    },
    {
      "id": "week",
      "name": "一周情书",
      "desc": "一周的情书",
      "mode": "rw",
      "define": {
        "type": "enum",
        "mapping": {
          "0": "Monday",
          "1": "Tuesday",
          "2": "Wednesday",
          "3": "Thursday",
          "4": "Friday",
          "5": "Saturday",
          "6": "Sunday"
        }
      },
      "required": false
    },
    {
      "id": "cloud_print",
      "name": "云打印机",
      "desc": "通过云端下发字符串控制打印机打印数据",
      "mode": "rw",
      "define": {
        "type": "string",
        "min": "0",
        "max": "2048"
      },
      "required": false
    }
  ],
  "events": [],
  "actions": []
}
```

