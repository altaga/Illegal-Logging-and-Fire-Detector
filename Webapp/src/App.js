import logo from './logo.svg';
import './App.css';
import IotReciever from "./components/iot-reciever-aws"
import MyMap from "./components/maps"
import React, { Component } from 'react';
import ReactDOM from "react-dom"
import { Card, Col, Row } from "reactstrap"
import styles from "./components/style-module"
import Alarm from "./components/Sounds/alarm.wav";
import { decode as atob, encode as btoa } from 'base-64'
import { isMobile } from "react-device-detect"

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      coord: [-99.074787, 19.460355],
      coords: [[-99.074787, 19.460355], [-99.07, 19.4605], [-99.0837, 19.460355]],
      colors: [styles.Blue, styles.Blue, styles.Blue],
      kind: [0, 0, 0],
      states: "Pending",
      loggings: "Pending",
      humans: "Pending",
      fires: "Pending"
    }
    this.callBackIoT = this.callBackIoT.bind(this);
    this.alarmAudio = new Audio(Alarm);
  }

  callBackIoT = (data) => {
    let temp = {
      name: JSON.parse(data[1]).name,
      payload: atob(JSON.parse(data[1]).payload),
    }
    if (temp.name === "ESP-LoraWAN") {
      this.setState({
        states : "Online"
      })
      if (temp.payload.length > 0) {
        console.log(temp)
        let temp2 = <MyMap
          coord={this.state.coord}
          coords={this.state.coords}
          colors={this.state.colors}
          kind={this.state.kind}
          zoom={14}
        />
        if (temp.payload === "0") {
          temp2 = <MyMap
            coord={this.state.coord}
            coords={this.state.coords}
            colors={[styles.Blue, styles.Blue, styles.Blue]}
            kind={[0, 0, 0]}
            zoom={14}
          />
          this.setState({
            humans: "Undetected",
            loggings: "Undetected",
            fires: "Undetected"
          })
          this.alarmAudio.pause();
        }
        else if (temp.payload === "1") {
          temp2 = <MyMap
            coord={this.state.coord}
            coords={this.state.coords}
            colors={[styles.Yellow, styles.Blue, styles.Blue]}
            kind={[1, 0, 0]}
            zoom={14}
          />
          this.setState({
            humans: "Detected",
            loggings: "Undetected",
            fires: "Undetected"
          })
          this.alarmAudio.play();
        }
        else if (temp.payload === "2") {
          temp2 = <MyMap
            coord={this.state.coord}
            coords={this.state.coords}
            colors={[styles.Red, styles.Blue, styles.Blue]}
            kind={[2, 0, 0]}
            zoom={14}
          />
          this.setState({
            humans: "Undetected",
            loggings: "Detected",
            fires: "Undetected"
          })
          this.alarmAudio.play();
        }
        else if (temp.payload === "3") {
          temp2 = <MyMap
            coord={this.state.coord}
            coords={this.state.coords}
            colors={[styles.Magenta, styles.Blue, styles.Blue]}
            kind={[3, 0, 0]}
            zoom={14}
          />
          this.setState({
            humans: "Undetected",
            loggings: "Undetected",
            fires: "Detected"
          })
          this.alarmAudio.play();
        }
        ReactDOM.unmountComponentAtNode(document.getElementById("map-zone"))
        ReactDOM.render(temp2, document.getElementById("map-zone"))
      }
    }
  }

  componentDidMount() {
    let maps = <MyMap
      coord={this.state.coord}
      coords={this.state.coords}
      colors={this.state.colors}
      kind={this.state.kind}
      zoom={14}
    />
    ReactDOM.render(maps, document.getElementById("map-zone"))
  }

  render() {
    let tempTEMP = "Pending"
    let logos = <img src={logo} alt="logo" width="100" height="100" className="right" />
    if (this.state.temperature !== "Pending") {
      tempTEMP = Math.round(((this.state.temperature * 1.8 + 32) + Number.EPSILON) * 100) / 100
    }
    return (
      <div className="App">
        <IotReciever sub_topics={["/helium/devices"]} callback={this.callBackIoT} />
        <Col style={{ background: "#f5f4f5", marginTop:"10px" }}>
          <Row md="2">
            <Col xs="3">
              {
                logos
              }
            </Col>
            <Col xs="8" style={{
              fontSize: "1.5rem",
              marginLeft: "10px",
            }}>
              Illegal Logging Detector and Fire Detector
            </Col>
          </Row>
        </Col>
        <hr />
        <Card style={{ padding: "10px" }}>
          <div id="map-zone" />
        </Card>
        <hr />
        <Row md="2" style={{ color: "black", fontSize: "1.3rem", margin:"10px" }}>
          <Col xs="6">
            <Card style={{ padding: "10px", borderTopLeftRadius: "30px", borderBottomRightRadius: "30px" }}>
              {"Device:"}
              <br />
              {this.state.states}
            </Card>
          </Col>
          <Col xs="6">
            <Card style={{ padding: "10px", borderTopLeftRadius: "30px", borderBottomRightRadius: "30px" }}>
              {"Humans:"}
              <br />
              {this.state.humans}
            </Card>
          </Col>
          <br />
          <br />
          <br />
          <Col xs="6">
            <Card style={{ padding: "10px", borderTopLeftRadius: "30px", borderBottomRightRadius: "30px" }}>
              {"Logging:"}
              <br />
              {this.state.loggings}
            </Card>
          </Col>
          <Col xs="6">
            <Card style={{ padding: "10px", borderTopLeftRadius: "30px", borderBottomRightRadius: "30px" }}>
              {"Fire:"}
              <br />
              {this.state.fires}
            </Card>
          </Col>
        </Row>
      </div>
    );
  }
}

export default App;