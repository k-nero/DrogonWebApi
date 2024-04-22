import { AddressInfo } from "net";
import path from 'path';
import express, { Request, Response, NextFunction } from "express";
const debug = require('debug')('my express app');

const app: Express = express();

import fileRouter = require("./routes/file");
import { Express } from "express";
import cors = require("cors");

app.use(cors());

app.use(express.static(path.join(__dirname, 'public')));
//app.use(express.static(path.join(__dirname, 'uploads')));

app.use('/files', fileRouter);

// catch 404 and forward to error handler
app.use((req, res, next) => {
    const err = new Error('Not Found');
    err[ 'status' ] = 404;
    next(err);
});

// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
    app.use((err: Error, req: Request, res: Response, next: NextFunction)  => { // eslint-disable-line @typescript-eslint/no-unused-vars
        res.status(err[ 'status' ] || 500);
        res.send({
            message: err.message,
            error: err
        });
    });
}

// production error handler
// no stacktrace leaked to user
app.use((err: any, req: Request, res: Response, next: NextFunction) => { // eslint-disable-line @typescript-eslint/no-unused-vars
    res.status(err.status || 500);
    res.send({
        message: err.message,
        error: {}
    });
});

app.set('port', process.env.PORT || 3000);

const server = app.listen(app.get('port'), function () {
    debug(`Express server listening on port ${(server.address() as AddressInfo).port}`);
});
