import { Circle as CircleStyle, Fill, Stroke, Style } from 'ol/style';

let styles = {
    'Point': new Style({
      image: new CircleStyle({
        radius: 10,
        fill: null,
        stroke: new Stroke({
          color: 'magenta',
        }),
      }),
    }),
    'Polygon': new Style({
      stroke: new Stroke({
        color: 'blue',
        lineDash: [4],
        width: 3,
      }),
      fill: new Fill({
        color: 'rgba(0, 0, 255, 0.1)',
      }),
    }),
    'Blue': new Style({
      stroke: new Stroke({
        color: 'blue',
        width: 1,
      }),
      fill: new Fill({
        color: 'rgba(0, 0, 255, 0.1)',
      }),
    }),
    'Yellow': new Style({
        stroke: new Stroke({
          color: 'yellow',
          width: 1,
        }),
        fill: new Fill({
          color: 'rgba(255, 255, 0, 0.5)',
        }),
      }),
    'Red': new Style({
      stroke: new Stroke({
        color: 'red',
        width: 1,
      }),
      fill: new Fill({
        color: 'rgba(255, 0, 0, 1)',
      }),
    }),
    'Magenta': new Style({
      stroke: new Stroke({
        color: '#663399',
        width: 1,
      }),
      fill: new Fill({
        color: 'rgba(102, 51, 153, 0.8)',
      }),
    })
  };

  export default styles