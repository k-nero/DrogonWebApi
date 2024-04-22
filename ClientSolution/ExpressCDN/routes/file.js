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
        cb(null, file.originalname);
    }
});
router.get("/files/:filename", function (req, res, next) {
    // const filename = req.params.filename;
    // const filepath = path.join(__dirname, "../uploads", filename);
    // const filestream = fs.createReadStream (filepath);
    // filestream.pipe(res);
    const filename = req.params.filename;
    const filepath = path.join(__dirname, "../uploads", filename);
    res.sendFile(filepath);
});
router.put("/uploads", function (req, res, next) {
    const upload = multer({ storage: storage }).single("file");
    upload(req, res, function (err) {
        if (err) {
            console.log(err);
            return res.status(500).json({ error: err.json });
        }
        res.status(200).send("File uploaded successfully");
    });
});
module.exports = router;
//# sourceMappingURL=file.js.map