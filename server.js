import { createRequire } from 'module';
const require = createRequire(import.meta.url);
import express from 'express'
import path from 'path'

const bodyParser = require("body-parser");
const addon = require('./build/Release/addon.node');
const exp = express();
const PORT = process.env.PORT || 3000;
const urlencodedParser = bodyParser.urlencoded({extended: false});


var ch = addon.check(); //получение строки всех имен пользователей
var ch = ch.split('='); //создание массива имен пользователей

exp.get('/',urlencodedParser,(req,res) => {
    res.sendFile(path.resolve(path.resolve(),'pages','index.html'))
})

exp.post('/', urlencodedParser,(req, res) => {
    if(!req.body) return res.sendStatus(400);
    console.log(req.body);

    //проверка наличия введенного имени
    if (ch.includes(req.body.userName)) res.send("Есть такой пользователь");
    else res.send("Нет такого пользователя");
});

exp.listen(PORT, ()=>{console.log('start')})
