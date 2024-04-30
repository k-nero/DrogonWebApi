import { AudioRecorder } from "react-audio-voice-recorder";
import React from "react";
import { Select } from "antd";

function AudioMessageModal({ handleVoiceRecordingComplete} : { handleVoiceRecordingComplete: (blob: Blob) => void })
{
    const [availableDevices, setAvailableDevices] = React.useState<MediaDeviceInfo[]>([]);
    const [selectedDevice, setSelectedDevice] = React.useState<MediaDeviceInfo>();

    function deviceFilter(devices: MediaDeviceInfo[])
    {
        // const audioInput = devices.filter((d) => d.kind === "audioinput");
        // let audioOptions: MediaDeviceInfo[] = [];
        // audioInput.map((d) => {
        //     if(d.deviceId === "default")
        //     {
        //         audioOptions.unshift(d);
        //     }
        //
        //     if(d.deviceId !== "default" && audioOptions.findIndex((a) => a.groupId === d.groupId && a.deviceId !== "default") === -1)
        //     {
        //         audioOptions.push(d);
        //     }
        //
        // });
        return devices.filter((d, i, s) => d.kind === "audioinput" && s.findIndex((a) => a.groupId === d.groupId) === i);
    }

    React.useEffect(() => {
        navigator.mediaDevices.enumerateDevices().then((devices) => {
           if(devices[0].deviceId === "")
           {
               //prompt user to allow microphone access
                navigator.mediaDevices.getUserMedia({ audio: true }).then(() => {
                    navigator.mediaDevices.enumerateDevices().then((devices) => {
                        setAvailableDevices(deviceFilter(devices));
                        setSelectedDevice(devices.find((d) => d.deviceId === "default"));
                    });
                });
           }
            setAvailableDevices(deviceFilter(devices));
            setSelectedDevice(devices.find((d) => d.deviceId === "default"));
        });
    }, []);


    return (<div className="text-center">
        <p className="">Press the button to start recording</p>
        <div className="mt-4">
            <Select options={availableDevices.map((d) => {
                return {
                    label: d.label,
                    value: d.groupId
                };
            })}
                    value = {
                         selectedDevice?.groupId
                    }
                    defaultActiveFirstOption={true}
                    onSelect={(value) => {
                        const selected = availableDevices.find((d) => {

                            return d.groupId === value;

                        });
                        setSelectedDevice(selected);
                    }}
                    className="w-1/2 m-auto" placeholder="Select an audio input device"/>
        </div>


        <div className="m-auto w-fit h-fit mt-4">
            <AudioRecorder onRecordingComplete={handleVoiceRecordingComplete}
                           audioTrackConstraints={{
                               noiseSuppression: true,
                               echoCancellation: true,
                               autoGainControl: true,
                               sampleRate: 44100,
                               groupId: selectedDevice?.groupId,
                               deviceId: selectedDevice?.deviceId,
                           }}
                           onNotAllowedOrFound={(err) => console.table(err)}
                           downloadOnSavePress={false}
                           downloadFileExtension="mp3"
                           mediaRecorderOptions={{
                               audioBitsPerSecond: 128000
                           }}
                           showVisualizer={true}
            />
        </div>
    </div>);
}

export default AudioMessageModal;
