const express = require("express");
const path = require("path");
const cookieParser = require("cookie-parser");
const logger = require("morgan");
const cors = require("cors");
const fileRouter = require("./routes/file");
const bodyParser = require("body-parser");

const app = express();

app.use(bodyParser.json({limit: '1gb'}));
app.use(bodyParser.urlencoded({extended:true, limit:'1gb'}));

app.use(logger('dev'));
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(express.static(path.join(__dirname, 'uploads')));

app.use('/', fileRouter);

module.exports = app;
