//
//  ViewController.swift
//  MacAudioStream
//
//  Created by MKolesov on 25/12/15.
//  Copyright © 2015 Michael Kolesov. All rights reserved.
//

import Cocoa

class ViewController: NSViewController, AudioStreamEngineDelegate {

    @IBOutlet weak var titleLabel: NSTextField!
    
    var audioStream: AudioStreamEngine!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        audioStream = AudioStreamEngine.sharedInstance()
        audioStream.delegate = self
    }

    override var representedObject: AnyObject? {
        didSet {
        // Update the view, if already loaded.
        }
    }

    @IBAction func playAction(_ sender: AnyObject) {
        
        print("play")
        if(audioStream.playing) {
            return
        }
        
        audioStream.start(withURL: "http://91.190.117.131:8000/live")
    }

    @IBAction func stopAction(_ sender: AnyObject) {
        print("stop")
        audioStream.stop()
    }
    
    //MARK: AudioStreamEngine delegate
    
    func audioStreamEngineDidUpdateTitle(_ title: String!) {
        
        print(title)
        
        titleLabel.stringValue = title
    }
}
