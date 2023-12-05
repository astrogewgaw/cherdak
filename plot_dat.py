import typer
import warnings
import pabo as pb
import numpy as np
import proplot as pplt

from rich import print
from rich.table import Table
from rich.panel import Panel

warnings.filterwarnings("ignore")

pplt.rc.update(
    {
        "font.size": 16,
        "title.loc": "ul",
        "cycle": "tab20b",
        "suptitle.pad": 20,
        "text.usetex": False,
        "axes.linewidth": 2.5,
        "tick.linewidth": 2.5,
        "subplots.tight": True,
        "subplots.share": True,
        "hatch.linewidth": 2.0,
        "subplots.refwidth": 5.0,
        "subplots.panelpad": 1.5,
        "agg.path.chunksize": 100000,
    }
)


def fprint(x: dict) -> None:
    grid = Table.grid(expand=True, padding=(0, 2, 0, 2))
    grid.add_column(justify="left", style="bold")
    grid.add_column(justify="right", style="italic")
    for key, value in x.items():
        grid.add_row(str(key), str(value))
    print(
        Panel(
            grid,
            padding=2,
            expand=False,
        )
    )


def main(fname: str):
    spec = pb.Spec(
        {
            "name": pb.PaddedString(128),
            "t0": pb.Float(4),
            "t1": pb.Float(4),
            "tsamp": pb.Float(4),
            "f1": pb.Float(4),
            "f2": pb.Float(4),
            "nchan": pb.Int(4),
            "postype": pb.Int(4),
            "raj_rad": pb.Float(4),
            "dec_rad": pb.Float(4),
            "useangle": pb.Int(4),
            "initialseed": pb.Int(8),
            "writelabels": pb.Int(4),
        }
    )

    with open(fname, "rb") as f:
        skip = f.read(64 + 128 + 4 * 11 + 8)
        meta = spec.parse(skip[64:])
        data = np.fromfile(f, dtype=np.float32)
    fprint(meta)

    transformed = np.fliplr(data.reshape(-1, meta["nchan"]))

    fig = pplt.figure()
    ax = fig.subplots()
    ax[0].imshow(
        transformed.T,
        aspect="auto",
        interpolation="none",
    )
    pplt.show()


if __name__ == "__main__":
    typer.run(main)
