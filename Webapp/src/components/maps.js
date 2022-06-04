import React from 'react';
import '../App.css';
import Map from "./Maps/Map";
import { Layers, TileLayer, VectorLayer } from "./Maps/Layers";
import { osm, vector } from "./Maps/Source";
import { fromLonLat, get } from 'ol/proj';
import GeoJSON from 'ol/format/GeoJSON';
import { Controls, FullScreenControl } from "./Maps/Controls";

function treeMarker(lon, lat, size, kind) {
    let geojsonObject;
    if (kind === 0) {
        geojsonObject = {
            "type": "FeatureCollection",
            "features": [
                {
                    "type": "Feature",
                    "properties": {
                        "kind": "county",
                        "name": "Wyandotte",
                        "state": "KS"
                    },
                    "geometry": {
                        "type": "MultiPolygon",
                        "coordinates": [
                            [
                                [
                                    [
                                        lon - 0 * size,
                                        lat + 1 * size
                                    ],
                                    [
                                        lon - 0.6 * size,
                                        lat + 0 * size
                                    ],
                                    [
                                        lon + 0.6 * size,
                                        lat + 0 * size
                                    ]
                                ]
                            ]
                        ]
                    }
                }
            ]
        }
    }
    else if (kind === 1) {
        geojsonObject = {
            "type": "FeatureCollection",
            "features": [
                {
                    "type": "Feature",
                    "properties": {
                        "kind": "county",
                        "name": "Wyandotte",
                        "state": "KS"
                    },
                    "geometry": {
                        "type": "MultiPolygon",
                        "coordinates": [
                            [
                                [
                                    [
                                        lon + 0.6 * size,
                                        lat + 0.6 * size
                                    ],
                                    [
                                        lon - 0.6 * size,
                                        lat + 0.6 * size
                                    ],
                                    [
                                        lon - 0.6 * size,
                                        lat - 0.6 * size
                                    ],
                                    [
                                        lon + 0.6 * size,
                                        lat - 0.6 * size
                                    ],
                                ]
                            ]
                        ]
                    }
                }
            ]
        }
    }
    else if (kind === 2) {
        geojsonObject = {
            "type": "FeatureCollection",
            "features": [
                {
                    "type": "Feature",
                    "properties": {
                        "kind": "county",
                        "name": "Wyandotte",
                        "state": "KS"
                    },
                    "geometry": {
                        "type": "MultiPolygon",
                        "coordinates": [
                            [
                                [
                                    [
                                        lon + Math.sin(0) * size,
                                        lat + Math.cos(0) * size*0.9
                                    ],
                                    [
                                        lon + Math.sin((360/5)*1*(2*Math.PI/360)) * size,
                                        lat + Math.cos((360/5)*1*(2*Math.PI/360)) * size*0.9
                                    ],
                                    [
                                        lon + Math.sin((360/5)*2*(2*Math.PI/360)) * size,
                                        lat + Math.cos((360/5)*2*(2*Math.PI/360)) * size*0.9
                                    ],
                                    [
                                        lon + Math.sin((360/5)*3*(2*Math.PI/360)) * size,
                                        lat + Math.cos((360/5)*3*(2*Math.PI/360)) * size*0.9
                                    ],
                                    [
                                        lon + Math.sin((360/5)*4*(2*Math.PI/360)) * size,
                                        lat + Math.cos((360/5)*4*(2*Math.PI/360)) * size*0.9
                                    ],
                                ]
                            ]
                        ]
                    }
                }
            ]
        }
    }
    else if (kind === 3) {
        geojsonObject = {
            "type": "FeatureCollection",
            "features": [
                {
                    "type": "Feature",
                    "properties": {
                        "kind": "county",
                        "name": "Wyandotte",
                        "state": "KS"
                    },
                    "geometry": {
                        "type": "MultiPolygon",
                        "coordinates": [
                            [
                                [
                                    [
                                        lon + Math.sin(0) * size,
                                        lat + Math.cos(0) * size*0.9
                                    ],
                                    [
                                        lon + Math.sin((360/6)*1*(2*Math.PI/360)) * size,
                                        lat + Math.cos((360/6)*1*(2*Math.PI/360)) * size*0.9
                                    ],
                                    [
                                        lon + Math.sin((360/6)*2*(2*Math.PI/360)) * size,
                                        lat + Math.cos((360/6)*2*(2*Math.PI/360)) * size*0.9
                                    ],
                                    [
                                        lon + Math.sin((360/6)*3*(2*Math.PI/360)) * size,
                                        lat + Math.cos((360/6)*3*(2*Math.PI/360)) * size*0.9
                                    ],
                                    [
                                        lon + Math.sin((360/6)*4*(2*Math.PI/360)) * size,
                                        lat + Math.cos((360/6)*4*(2*Math.PI/360)) * size*0.9
                                    ],
                                    [
                                        lon + Math.sin((360/6)*5*(2*Math.PI/360)) * size,
                                        lat + Math.cos((360/6)*5*(2*Math.PI/360)) * size*0.9
                                    ],
                                ]
                            ]
                        ]
                    }
                }
            ]
        }
    }
    return geojsonObject
}

class MyMap extends React.Component {
    componentDidUpdate() {

    }

    render() {
        let i = 0;
        return (
            <Map center={fromLonLat(this.props.coord)} zoom={this.props.zoom}>
                <Layers>
                    <TileLayer
                        source={osm()}
                        zIndex={0}
                    />
                    {
                        this.props.coords.map((number) =>
                            <div key={i}>
                                <VectorLayer
                                    source={vector({ features: new GeoJSON().readFeatures(treeMarker(number[0], number[1], 0.002,this.props.kind[i]), { featureProjection: get('EPSG:3857') }) })}
                                    style={this.props.colors[i++]}
                                />
                            </div>
                        )
                    }
                </Layers>
                <Controls>
                    <FullScreenControl />
                </Controls>
            </Map>
        );
    }
}

export default MyMap;