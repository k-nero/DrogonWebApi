"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
// import express, { Request, Response, NextFunction } from "express";
// const debug = require('debug')('my express app');
const cors = require("cors");
const hyper_express_1 = __importDefault(require("hyper-express"));
const fileRouter = require("./routes/file");
const webserver = new hyper_express_1.default.Server({
    cert_file_name: "certificate.crt",
    key_file_name: "private.key"
});
webserver.use("/files", fileRouter);
webserver.set_not_found_handler((req, res) => {
    res.status(404).send("Not Found");
});
webserver.set_error_handler((request, response, error) => {
    response.status(500).send(JSON.stringify({
        message: error.message
    }));
});
webserver.use(cors());
webserver.get("/hello", (req, res) => {
    res.send("Hello World");
});
webserver.listen(3000, () => {
    console.log("Server is running on port 3000");
}).then(r => {
});
// const app: Express = express();
//
//
//
//
// import { Express } from "express";
// import cors = require("cors");
// import * as https from "node:https";
// import * as fs from "node:fs";
//
// app.use(cors());
//
// app.use('/files', fileRouter);
//
// // catch 404 and forward to error handler
// app.use((req, res, next) => {
//     const err = new Error('Not Found');
//     err[ 'status' ] = 404;
//     next(err);
// });
//
// // error handlers
//
// // development error handler
// // will print stacktrace
// if (app.get('env') === 'development') {
//     app.use((err: Error, req: Request, res: Response, next: NextFunction)  => { // eslint-disable-line @typescript-eslint/no-unused-vars
//         res.status(err[ 'status' ] || 500);
//         res.send({
//             message: err.message,
//             error: err
//         });
//     });
// }
//
// // production error handler
// // no stacktrace leaked to user
// app.use((err: any, req: any, res: any, next: any) => { // eslint-disable-line @typescript-eslint/no-unused-vars
//     res.status(err.status || 500);
//     res.send({
//         message: err.message,
//         error: {}
//     });
// });
//
// app.set('port', process.env.PORT || 3000);
// const cert = fs.readFileSync("./certificate.crt");
// const key = fs.readFileSync("./private.key");
//
// const server = https.createServer({
//     key: key,
//     cert: cert
// }, app);
//
// server.listen(app.get('port'), () => {
//     debug('Express server listening on port ' + (server.address() as AddressInfo).port);
// });
//# sourceMappingURL=app.js.map