class Waypoint extends DraggablePoint {
    /**
     * @param {Cesium.Viewr} viewer
     * @param {JSON} waypointData
     * @param {Cesium.Color} color
     */
    constructor(viewer, waypointData, color = Cesium.Color.WHITE) {
        super(viewer);

        // Data
        this.update(waypointData);

        // Visual
        this.normalScale = 1.0;
        this.hoveredScale = 1.2;
        // this.visible = true;

        var that = this;

        // SVG billboard with label
        this.point = viewer.entities.add({
            position: new Cesium.CallbackProperty(() => { return that.position; }, false),
            billboard: {
                image: "./icons/wpt.svg",
                color: color,
                disableDepthTestDistance: Number.POSITIVE_INFINITY,
                scale: this.normalScale
            },
            label: {
                text: new Cesium.CallbackProperty(() => { return that.name; }, false),
                show: false,
                showBackground: true,
                disableDepthTestDistance: Number.POSITIVE_INFINITY,
                pixelOffset: new Cesium.Cartesian2(0, -25),
                font: "13px Helvetica"
            }
        });

        // Dash line to the terrain
        this.pylon = this.viewer.entities.add({
             polyline: {
                 positions: new Cesium.CallbackProperty(() => {
                     return [that.position, that.terrainPosition];
                 }, false),
                 width: 1,
                 arcType: Cesium.ArcType.NONE,
                 material: new Cesium.PolylineDashMaterialProperty(Cesium.Color.GAINSBORO)
             }
        });

        // DraggablePoint point on the ground
        this.groundPoint = this.viewer.entities.add({
            position: new Cesium.CallbackProperty(() => { return that.terrainPosition }, false),
            show: false,
            point: {
                pixelSize: this.pointPixelSize,
                color: Cesium.Color.CADETBLUE
            }
        });
    }

    clear() {
        this.viewer.entities.remove(this.point);
        this.viewer.entities.remove(this.pylon);
        this.viewer.entities.remove(this.groundPoint);
    }

    /**
     * @param {JSON} waypointData - JSON, must contain latitude, longitude, altitude (AMSL)
     */
    update(waypointData) {
        this.waypointData = waypointData;
        var params = waypointData.params;

        this.name = waypointData.name;

        this.updatePosition(Cesium.Cartesian3.fromDegrees(params.longitude,
                                                          params.latitude,
                                                          params.altitude));
    }

    updatePosition(cartesian, saveAltitude = false) {
        // Update terrainPosition from the terrainProvider
        var cartographic = Cesium.Cartographic.fromCartesian(cartesian);

        this.terrainPosition = cartesian;
        if (saveAltitude) {
            this.position = Cesium.Cartesian3.fromDegrees(
                Cesium.Math.toDegrees(cartographic.longitude),
                Cesium.Math.toDegrees(cartographic.latitude),
                this.waypointData.params.altitude);
        } else {
            this.position = cartesian;
        }

        var that = this;
        var promise = Cesium.sampleTerrainMostDetailed(this.viewer.terrainProvider, [cartographic]);
        Cesium.when(promise, updatedPositions => {
                        that.terrainPosition = Cesium.Cartographic.toCartesian(cartographic);
                    });
    }

    flyTo() {
        this.viewer.flyTo(this.point);
    }

    setEditMode(edit) {
        this.point.label.show = edit;
        this.groundPoint.show = edit;
    }

    // TODO: set hovered for terrain point and waypoint separatly
    setHovered(hovered) {
        this.groundPoint.point.pixelSize = hovered ? this.hoveredPointPixelSize : this.pointPixelSize;
    }

    checkMatch(objects) {
        var result = null;
        objects.forEach(object => {
            // check ground pointa
            if (this.groundPoint === object.id)
                 result = this.point;
        });
        return result;
    }
}
