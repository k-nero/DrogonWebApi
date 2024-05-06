"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
//const router = express.Router();
const path = require("path");
const hyper_express_1 = __importDefault(require("hyper-express"));
const fs = __importStar(require("node:fs"));
const router = new hyper_express_1.default.Router();
router.get("/:filename", function (req, res, next) {
    // const filename = req.params.filename;
    // const filepath = path.join(__dirname, "../uploads", filename);
    // const filestream = fs.createReadStream (filepath);
    // filestream.pipe(res);
    try {
        const filename = req.params.filename;
        const filepath = path.join(__dirname, "../uploads", filename);
        //check if file exists
        if (!fs.existsSync(filepath)) {
            res.status(404).send("File not found");
            return;
        }
        // if(filepath.split(".")[1] === "mp4" || filepath.split(".")[1] === "webm")
        // {
        //     res.setHeader("Cross-Origin-Resource-Policy", "cross-origin");
        //     //res.setHeader("Content-disposition", "attachment; filename=" + filename);
        //
        //     const filestream = fs.createReadStream(filepath);
        //
        //     res.stream(filestream).then(r => {
        //
        //     });
        // }
        res.setHeader("Cross-Origin-Resource-Policy", "cross-origin");
        res.setHeader("Content-disposition", "attachment; filename=" + filename);
        res.sendFile(filepath);
    }
    catch (error) {
        res.status(500).send("Error downloading file");
    }
});
router.put("/uploads", function (req, res, next) {
    return __awaiter(this, void 0, void 0, function* () {
        let savePath;
        let originalFilename;
        let fileName;
        try {
            yield req.multipart((field) => __awaiter(this, void 0, void 0, function* () {
                if (field.file) {
                    originalFilename = field.file.name;
                    //randomize the filename with date
                    fileName = new Date().getTime() + field.file.name.split(".")[1];
                    savePath = path.join(__dirname, "../uploads", fileName);
                    yield field.write(savePath);
                }
            }));
        }
        catch (error) {
            res.status(500).send("Error uploading file");
            return;
        }
        if (savePath) {
            res.status(200).send(JSON.stringify({
                message: "File is uploaded",
                originalFileName: originalFilename,
                filepath: fileName
            }));
        }
    });
});
module.exports = router;
//# sourceMappingURL=file.js.map