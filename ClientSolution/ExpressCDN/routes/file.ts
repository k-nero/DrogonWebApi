//import express = require("express");
//const router = express.Router();
import path = require("path");
import HyperExpress from "hyper-express";
import * as fs from "node:fs";

const router = new HyperExpress.Router();

router.get("/:filename", function(req, res, next)
{
    // const filename = req.params.filename;
    // const filepath = path.join(__dirname, "../uploads", filename);
    // const filestream = fs.createReadStream (filepath);
    // filestream.pipe(res);
    try
    {
        const filename = req.params.filename;
        const filepath = path.join(__dirname, "../uploads", filename);

        //check if file exists
        if(!fs.existsSync(filepath))
        {
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
    catch (error)
    {
        res.status(500).send("Error downloading file");
    }

});

router.put("/uploads", async function(req, res, next) {

    let savePath: string;
    let originalFilename: string;
    let fileName: string;
    try
    {
        await req.multipart(async (field) => {
            if (field.file)
            {
                originalFilename = field.file.name;
                //randomize the filename with date
                fileName = new Date().getTime() + field.file.name.split(".")[1];
                savePath = path.join(__dirname, "../uploads", fileName);
                await field.write(savePath);
            }
        })
    }
    catch (error)
    {
        res.status(500).send("Error uploading file");
        return;
    }

    if(savePath)
    {
        res.status(200).send(JSON.stringify({
            message: "File is uploaded",
            originalFileName: originalFilename,
            filepath: fileName
        }));
    }
})

router.get("/tesseract/:filename", function(req, res, next) {
    try
    {
        const filename = req.path_parameters.filename;
        const filepath = path.join(__dirname, "../tesseract", filename);
        console.log(filepath);

        //check if file exists
        if(!fs.existsSync(filepath))
        {
            res.status(404).send("File not found");
            return;
        }
        res.setHeader("Cross-Origin-Resource-Policy", "cross-origin");
        res.setHeader("Content-disposition", "attachment; filename=" + filename);
        res.sendFile(filepath);
    }
    catch (error)
    {
        res.status(500).send("Error downloading file");
    }
})

export = router;
