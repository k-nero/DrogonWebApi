"use strict";
const express = require("express");
const router = express.Router();
const path = require("path");
const multer = require("multer");
const storage = multer.diskStorage({
    destination: function (req, file, cb) {
        cb(null, "uploads/");
    },
    filename: function (req, file, cb) {
        const newFileName = Date.now() + path.extname(file.originalname);
        cb(null, newFileName);
    }
});
router.get("/:filename", function (req, res, next) {
    // const filename = req.params.filename;
    // const filepath = path.join(__dirname, "../uploads", filename);
    // const filestream = fs.createReadStream (filepath);
    // filestream.pipe(res);
    const filename = req.params.filename;
    const filepath = path.join(__dirname, "../uploads", filename);
    res.setHeader("Cross-Origin-Resource-Policy", "cross-origin");
    res.sendFile(filepath);
});
router.put("/uploads", function (req, res, next) {
    const upload = multer({ storage: storage }).single("file");
    upload(req, res, function (err) {
        if (err) {
            console.log(err);
            return res.status(500).json({ error: err.json });
        }
        res.status(200).send({
            message: "File is uploaded",
            filename: req.file.filename
        });
    });
});
module.exports = router;
//# sourceMappingURL=file.js.map